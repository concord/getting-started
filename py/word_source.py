#!/usr/bin/env python
from concord.computation import (
    Computation,
    Metadata
)
import sys
import time

def time_millis():
    return int(round(time.time() * 1000))


class WordSource(Computation):
    def __init__(self):
        self.words = ['foo', 'bar', 'baz', 'fiz', 'buzz']

    def sample(self):
        """returns a random word"""
        import random
        return random.choice(self.words)

    def init(self, ctx):
         print "Source initialized"
        ctx.set_timer('loop', time_millis())

    def process_timer(self, ctx, key, time):
        # stream, key, value. empty value, no need for val
        ctx.produce_record("words", self.sample(), '-')

        # emit records every 100ms
        ctx.set_timer("main_loop", time_millis() + 100)

    def process_record(self, ctx, record):
        raise Exception('process_record not implemented')

    def metadata(self):
        return Metadata(
            name='word-source',
            istreams=[],
            ostreams=['words'])

print "Main"
concord.computation.serve_computation(WordCounter())
