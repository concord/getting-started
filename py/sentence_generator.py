import sys
import time
import concord
from concord.computation import (
    Computation,
    Metadata,
    serve_computation
)

def time_millis():
    return int(round(time.time() * 1000))

class SentenceGenerator(Computation):
    def __init__(self):
        self.sentences = [ "How do I love thee? Let me count the ways.",
                           "I love thee to the depth and breadth and height",
                           "My soul can reach, when feeling out of sight",
                           "For the ends of being and ideal grace.",
                           "I love thee to the level of every day's",
                           "Most quiet need, by sun and candle-light.",
                           "I love thee freely, as men strive for right.",
                           "I love thee purely, as they turn from praise.",
                           "I love thee with the passion put to use",
                           "In my old griefs, and with my childhood's faith.",
                           "I love thee with a love I seemed to lose",
                           "With my lost saints. I love thee with the breath,",
                           "Smiles, tears, of all my life; and, if God choose,",
                           "I shall but love thee better after death." ]

    def sample(self):
        """returns a random word"""
        import random
        return random.choice(self.sentences)

    def init(self, ctx):
        self.concord_logger.info("Source initialized")
        ctx.set_timer('loop', time_millis())

    def process_timer(self, ctx, key, time):
        # stream, key, value. empty value, no need for val
        iterations = 10000
        while iterations > 0:
            iterations -= 1
            ctx.produce_record("sentences", self.sample(), '-')

        # emit records every 500ms
        ctx.set_timer("main_loop", time_millis())

    def process_record(self, ctx, record):
        raise Exception('process_record not implemented')

    def metadata(self):
        return Metadata(
            name='sentence-generator',
            istreams=[],
            ostreams=['sentences'])

serve_computation(SentenceGenerator())
