import sys
import concord
from concord.computation import (
    Computation,
    Metadata,
    serve_computation
)

class WordCounter(Computation):
    def __init__(self):
        self.dict = {}
        self.pidx = 0 # print index

    def init(self, ctx):
        print "Counter initialized"

    def process_timer(self, ctx, key, time):
        raise Exception('process_timer not implemented')

    def process_record(self, ctx, record):
        self.pidx += 1
        if self.dict.has_key(record.key):
            self.dict[record.key] += 1
        else:
            self.dict[record.key] = 1

        if (self.pidx % 1024) == 0:
            print self.dict

    def metadata(self):
        return Metadata(
            name='word-counter',
            istreams=['words'],
            ostreams=[])

print "Main"
serve_computation(WordCounter())
