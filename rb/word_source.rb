require 'concord'
require 'concord/utils'

DICTIONARY = ['foo', 'bar', 'baz', 'qux']

class WordSource
  # init takes one argument:
  # - context: the context object used to interact with the framework
  def init(context)
    Concord::Utils.log_to_stderr("Initialized Computation")
    context.set_timer('default', Concord::Utils.time_millis)
  end

  # process_timer takes three arguments:
  # - context: the context object used to interact with the framework
  # - key: the name of the timer that's being triggered
  # - time: the time the timer was scheduled to trigger at
  def process_timer(context, key, time)
    (0..1024).each do |i|
      context.produce_record('words', DICTIONARY.sample, '')
    end
    context.set_timer(key, Concord::Utils.time_millis + 5000)
  end

  # metadata takes no arguments, but expects a return value of a `Metadata`
  # object. check out the ruby client api documentation for more information.
  def metadata
    Concord::Utils.log_to_stderr("Metadata called")
    Concord::Metadata.new(name: 'word-source', ostreams: ['words'])
  end
end

Concord::Computation.serve(WordSource.new)
