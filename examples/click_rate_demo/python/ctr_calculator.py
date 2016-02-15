import time
import json
from concord.computation import (
    Computation,
    Metadata,
    serve_computation
)

def time_millis():
    return int(round(time.time() * 1000))

class CtrCalculator(Computation):
    def init(self, ctx):
        self.providerCtr = { }
        self.concord_logger.info("Initializing click-through-rate calculator")
        ctx.set_timer('loop', time_millis())

    def process_timer(self, ctx, key, time):
        def ctr(clicks, imps):
            if imps == 0 or clicks >= imps:
                return 100.0
            return (clicks / imps) * 100.0

        for provider_name, provider_stats in self.providerCtr.iteritems():
            clicks, impressions = provider_stats
            if clicks < impressions:
                self.concord_logger.info("Data for provider [" + provider_name +"]")
                self.concord_logger.info("Clicks: " + clicks)
                self.concord_logger.info("Impressions: " + impressions)
                self.concord_logger.info("Click Through Rate: " + ctr(clicks, impressions))

        self.providerCtr.clear()
        ctx.set_timer('loop', time_millis() + 1000)

    def process_record(self, ctx, record):
        provider = record.key
        payload = json.loads(record.value)

        clicks, impressions = (0, 0)
        if record.key in self.providerCtr:
            clicks, impressions = self.providerCtr[provider]

        if payload['type'] == 'CLICK':
            clicks += 1
        else if payload['type'] == 'IMPRESSION':
            impressions += 1
        else:
            raise Exception("Unhandled event intercepted")
        self.providerCtr[provider] = (clicks, impressions)

    def metadata(self):
        return Metadata(
            name='ctr-calculator',
            istreams=[ ('impressions', StreamGrouping::GROUP_BY),
                        ('clicks', StreamGrouping::GROUP_BY) ])

serve_computation(CtrCalculator())
