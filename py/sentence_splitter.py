import concord
from concord.computation import (
    Computation,
    Metadata,
    serve_computation
)

class SentenceSplitter(Computation):
    """ Simple computation that splits sentences into words"""
    def init(self, ctx):
        self.concord_logger.info("Computation initialized")

    def process_record(self, ctx, record):
        words = record.data.split()
        self.concord_logger.info(words)
        for word in words:
            ctx.produce_record('words', word, '-')

    def metadata(self):
        return Metadata(
            name='word-filter',
            istreams=['sentences'],
            ostreams=['words'])

serve_computation(SentenceSplitter())
