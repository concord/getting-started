require 'concord'
require 'concord/utils'

SENTENCES = [ "How do I love thee? Let me count the ways.",
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

class SentenceGenerator
  # init takes one argument:
  # - context: the context object used to interact with the framework
  def init(context)
    Concord::Utils.log_to_stderr("Initialized Computation")
    context.set_timer('default', Concord::Utils.time_millis)
  end

  # Use the destroy method to perform some cleanup before the framework
  # kills this process
  def destroy
    Concord::Utils.log_to_stderr("Destructing sentence generator")
  end

  # process_timer takes three arguments:
  # - context: the context object used to interact with the framework
  # - key: the name of the timer that's being triggered
  # - time: the time the timer was scheduled to trigger at
  def process_timer(context, key, time)
    (0..10000).each do |i|
      context.produce_record('words', SENTENCES.sample, '')
    end
    context.set_timer(key, Concord::Utils.time_millis)
  end

  # metadata takes no arguments, but expects a return value of a `Metadata`
  # object. check out the ruby client api documentation for more information.
  def metadata
    Concord::Utils.log_to_stderr("Metadata called")
    Concord::Metadata.new(name: 'word-source', ostreams: ['sentences'])
  end
end

Concord::Computation.serve(SentenceGenerator.new)
