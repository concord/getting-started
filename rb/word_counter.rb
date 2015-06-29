require 'concord'

def log(str)
  $stderr.puts str
  $stderr.flush
end

class WordCounter
  attr_accessor :words, :total_words

  def initialize()
    self.words = {}
    self.total_words = 0
  end

  def init(context)
    log "Initialized word counter"
  end

  def process_record(context, record)
    key = record.key
    self.words[key] ||= 0
    self.words[key] += 1
    if self.total_words % 1024 == 0
      log "Dumping words: #{self.words}"
    end
  end

  def metadata
    log "Metadata called"
    Concord::Metadata.new(name: 'word-counter',
                          istreams: [['words',
                           Concord::Thrift::StreamGrouping::GROUP_BY]])
  end

end

Concord::Computation.serve(WordCounter.new)
