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
      context.produce_record('words', DICTIONARY.sample, '')
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

Concord::Computation.serve(WordSource.new)
