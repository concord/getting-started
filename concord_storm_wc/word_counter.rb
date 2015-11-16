require 'concord'
require 'concord/utils'

class WordCounter
  attr_accessor :words, :total_words

  def initialize()
    self.words = {}
    self.total_words = 0
  end

  # init takes on argument:
  # - context: the context object provided by the framework, used to communicate
  #   with the framework
  def init(context)
    Concord::Utils.log("Initialized word counter")
  end

  # process_record takes two arguments:
  # - context: the context object used to interact with the framework
  # - record: the incoming record to be processed
  def process_record(context, record)
    key = record.key
    self.words[key] ||= 0
    self.words[key] += 1
    self.total_words += 1
    if self.total_words % 1024 == 0
      Concord::Utils.log("Dumping words: #{self.words}")
    end
  end

  # metadata takes no arguments, but expects a return value of a `Metadata`
  # object. check out the ruby client api documentation for more information.
  def metadata
    Concord::Utils.log("Metadata called")
    Concord::Metadata.new(name: 'word-counter',
                          istreams: [['words',
                           Concord::Thrift::StreamGrouping::GROUP_BY]])
  end

end

Concord::Computation.serve(WordCounter.new)
