"""
Operator: Join Winning Bids
Creator: Concord Systems, Inc
Date: 9.22.15
Description: Defines a new stream called 'winningbids' that is composed of
filtering losing bids from an input stream named 'bids' with impression data
from a stream called 'imps'.
"""

import concord
from cachetools import TTLCache
from concord.computation import (
    Computation,
    Metadata,
    serve_computation
)

class JoinWinningBids(Computation):
    """ Filters out losing bids from a stream of bids and impressions.
    Omit records that don't have a match within a 60s interval."""
    def __init__(self, maxsize, ttl, prune_time=5):
        self.prune_time = prune_time
        self.cache = TTLCache(maxsize, ttl)

    def init(self, ctx):
        self.concord_logger.info("Operator initialized")
        if self.prune_time > 0:
            ctx.set_timer('loop', time_time() * 1000)

    def process_timer(self, ctx, key, time):
        """ Prune the cache of expired items every 'prune_time' seconds.
        Otherwise this would only happen when mutating the cache"""
        self.cache.expire()
        ctx.set_timer('cleanup_loop', (time.time() + self.prune_time) * 1000)

    def process_record(self, ctx, record):
        """ With GROUP_BY routing strategy, it is guaranteed that the same
        key will be sent to the same operator, regardless of scaling"""
        if record.stream == 'bids':
            self.cache[record.key] = record.data
        elif record.stream == 'imps':
            bid = self.cache[record.key]
            if bid is not None:
                ctx.process_record('winningbids', record.key, '-')

    def metadata(self):
        return Metadata(
            name='filter-winning-bids',
            istreams=[('bids', StreamGrouping.GROUP_BY),
                      ('imps', StreamGrouping.GROUP_BY)],
            ostreams=['winningbids'])

serve_computation(FilterWinningBids(5000000, 60))
