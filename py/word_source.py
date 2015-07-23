import sys
import time
import concord
from concord.computation import (
    Computation,
    Metadata,
    serve_computation
)

import logging
logging.basicConfig()
logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)

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
        logger.info("Source initialized")
        ctx.set_timer('loop', time_millis())

    def process_timer(self, ctx, key, time):
        # stream, key, value. empty value, no need for val
        for _ in range(0, 1024):
            ctx.produce_record("words", self.sample(), '-')

        # emit records every 100ms
        ctx.set_timer("main_loop", time_millis() + 5000)

    def process_record(self, ctx, record):
        raise Exception('process_record not implemented')

    def metadata(self):
        return Metadata(
            name='word-source',
            istreams=[],
            ostreams=['words'])

logger.info("Main")
serve_computation(WordSource())
