#/usr/bin/env python
import concord.computation.(
    Computation,
    Metadata
)
import sys

class WordCounter(Computation):
    def __init__(self):
        self.dict = {}
        self.pidx = 0 # print index

    def init(self, ctx):
        print("Counter initialized", file=sys.stderr)

    def process_timer(self, ctx, key, time):
        raise Exception('process_timer not implemented')

    def process_record(self, ctx, record):
        self.pidx += 1
        self.dict[record.key] += 1
        if (self.pidx % 100) == 0:
            print(self.pidx, file=sys.stderr)

    def metadata(self):
        return Metadata(
            name='word-counter',
            istreams=['words'],
            ostreams=[])
print "foo"
concord.computation.serve_computation(WordCounter())
