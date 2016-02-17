import time
import json
from thrift.protocol import TBinaryProtocol
from thrift.transport import TTransport
from concord.computation import (
    Computation,
    Metadata,
    StreamGrouping,
    serve_computation
)
from click.ttypes import (
    AdEvent,
    StreamEvent
)

# Returns the time now in milliseconds
def time_millis():
    return int(round(time.time() * 1000))

# Method that deserializes bytes into a thrift struct
def bytes_to_thrift(bytes, thrift_struct):
    transportIn = TTransport.TMemoryBuffer(bytes)
    protocolIn = TBinaryProtocol.TBinaryProtocol(transportIn)
    thrift_struct.read(protocolIn)

class CtrCalculator(Computation):
    
    # init takes one argument:
    # - ctx: The context object used to interact with the framework
    def init(self, ctx):
        # Storage for in memory aggregation of click through rate metrics (per provider)
        self.providerCtr = { }
        self.concord_logger.info("Initializing click-through-rate calculator")

        # Arm timer to begin logging metrics after every second
        ctx.set_timer('loop', time_millis())

    # process_timer takes three arguments:
    # - ctx: The context object used to interact with the framework
    # - key: The name of the timer that has been triggered
    # - time: The time of the timer that was scheduled to trigger
    def process_timer(self, ctx, key, time):
        # Local method that calulcates click through rate
        def ctr(clicks, imps):
            if imps == 0 or clicks >= imps:
                return 100.0
            return (clicks / imps) * 100.0

        # Enumerate through in memory store of click metrics
        # Log metrics and send data downstream using provider name as the key
        for provider_name, provider_stats in self.providerCtr.iteritems():
            clicks, impressions = provider_stats
            if clicks < impressions:
                click_through_rate = ctr(clicks, impressions)
                self.concord_logger.info("Data for provider [" + provider_name +"]")
                self.concord_logger.info("Clicks: " + clicks)
                self.concord_logger.info("Impressions: " + impressions)
                self.concord_logger.info("Click Through Rate: " + click_through_rate)
                ctx.produce_record('click_through_rate', provider_name, click_through_rate)

        # Clear metrics and repeat this method in a second from now
        self.providerCtr.clear()
        ctx.set_timer('loop', time_millis() + 1000)

    # process_record takes two arguments:
    # - ctx: The context object used to interact with the framework
    # - record: The incoming record to be processed
    def process_record(self, ctx, record):
        provider = record.key
        
        # Deserialize upstream data using 'bytes_to_thrift' method
        payload = AdEvent()
        bytes_to_thrift(record.data, payload)

        # Query cache for metrics pertaining to the given provider..
        clicks, impressions = (0, 0)
        if record.key in self.providerCtr:
            clicks, impressions = self.providerCtr[provider]

        # Increment event count
        if payload.type == StreamEvent.CLICK:
            clicks += 1
        elif payload.type == StreamEvent.IMPRESSION:
            impressions += 1
        else:
            raise Exception("Unhandled event intercepted")
        
        # Update cache with freshest metric data
        self.providerCtr[provider] = (clicks, impressions)

    # metadata takes no arguments.
    # returns: An object of type 'Metadata'. This object has three named fields, name, istreams,
    # and ostreams. The data in this object will be used by the framework to determine what streams
    # this operator subscribes and/or publishes to.
    def metadata(self):
        # istreams is an array that expects tuples of a type: (string, StreamGrouping).
        # In this example there are multiple istreams, therefore this computation is performing
        # a type of stream join.
        
        # The GROUP_BY enum instructs the framework that an aggregation by key is to be performed.
        # This ensures that records with the same key are always sent to the same instance of
        # this operator.

        # By aggregating two streams by key we can be assured that records from any stream emitted
        # on a particular provider will always be sent to the same instance of CtrCalculator.
        return Metadata(
            name='ctr-calculator',
            istreams=[ ('impressions', StreamGrouping.GROUP_BY),
                        ('clicks', StreamGrouping.GROUP_BY) ])

# Initializes thrift server so the concord proxy can communicate with this process via thrift RPC.=
# Makes 'registerWithScheduler' RPC which begins a series of events that will place this operator into
# a concord topology.
serve_computation(CtrCalculator())
