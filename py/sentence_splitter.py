from concord.computation import (
    Computation,
    Metadata,
    serve_computation
)
from concord.internal.thrift.ttypes import StreamGrouping

class SentenceSplitter(Computation):
    def init(self, ctx):
        self.concord_logger.info("Splitter initialized")

    def process_timer(self, ctx, key, time):
        raise Exception('process_timer not implemented')

    def process_record(self, ctx, record):
        for word in record.key.split(" "):
            ctx.produce_record('words', word, '-')

    def metadata(self):
        return Metadata(
            name='sentence-splitter',
            istreams=['sentences'],
            ostreams=['words'])

serve_computation(SentenceSplitter())
