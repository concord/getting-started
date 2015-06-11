require 'concord'

def log(str)
  $stderr.puts str
  $stderr.flush
end

class WordCounter
  attr_accessor :words

  def initialize()
    self.words = {}
  end

  def init(ctx)
    log "Initialized word counter"
  end

  def process_record(ctx, record)
    key = record.key
    self.words[key] ||= 0
    self.words[key] += 1
    log "Got key: #{key}"
  end

  def metadata
    log "Metadata called"
    Concord::Metadata.new(name: 'word-counter',
                          istreams: [['words',
                           Concord::Thrift::StreamGrouping::GROUP_BY]])

  end
end

Concord::Computation.serve(WordCounter.new)
