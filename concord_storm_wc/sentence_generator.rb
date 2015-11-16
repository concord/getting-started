require 'concord'
require 'concord/utils'

class SentenceGenerator
  NOUNS = ['gentelmen', 'dog', 'vehicle', 'fox', 'table']
  ADJECTIVES = ['quickly', 'quietly', 'slowly', 'faintly', 'slightly']

  # Method returns a random sentence made up of words from the
  # class variable NOUNS and ADJECTIVES
  def randomSentence
    "The " + NOUNS.sample + " appeared " + ADJECTIVES.sample
  end

  # init takes one argument:
  # - context: the context object used to interact with the framework
  def init(context)
    Concord::Utils.log("Initialized Computation")
    context.set_timer('default', Concord::Utils.time_millis)
  end

  # process_timer takes three arguments:
  # - context: the context object used to interact with the framework
  # - key: the name of the timer that's being triggered
  # - time: the time the timer was scheduled to trigger at
  def process_timer(context, key, time)
    (0..1024).each do |i|
      context.produce_record('sentences', '', self.randomSentence)
    end
    context.set_timer(key, Concord::Utils.time_millis + 5000)
  end

  # metadata takes no arguments, but expects a return value of a `Metadata`
  # object. check out the ruby client api documentation for more information.
  def metadata
    Concord::Utils.log("Metadata called")
    Concord::Metadata.new(name: 'sentence-generator', ostreams: ['sentences'])
  end
end

Concord::Computation.serve(SentenceGenerator.new)
