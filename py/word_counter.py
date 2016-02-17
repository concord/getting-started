import sys
import concord
from concord.computation import (
    Computation,
    Metadata,
    StreamGrouping,
    serve_computation
)

class WordCounter(Computation):
    def __init__(self):
        self.dict = {}
        self.pidx = 0 # print index

    def init(self, ctx):
        self.concord_logger.info("Counter initialized")

    def process_timer(self, ctx, key, time):
        raise Exception('process_timer not implemented')

    def process_record(self, ctx, record):
        self.pidx += 1
        if self.dict.has_key(record.key):
            self.dict[record.key] += 1
        else:
            self.dict[record.key] = 1

        if (self.pidx % 10000) == 0:
            self.concord_logger.info(self.dict)

    def metadata(self):
        return Metadata(
            name='word-counter',
            istreams=[('words', StreamGrouping.GROUP_BY)],
            ostreams=[])

serve_computation(WordCounter())
