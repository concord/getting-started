require 'concord'
require 'concord/utils'

class SentenceSplitter
  def init(context)
    Concord::Utils.log_to_stderr("Initialized Splitter computation")
  end

  def process_record(context, record)
    record.key.split do |word|
      context.produce_record('words', word, '-')
    end
  end

  def metadata
    Concord::Metadata.new(name: 'sentence-splitter',
                          istreams: [['sentences', Concord::Thrift::StreamGrouping::SHUFFLE]]
                          ostreams: ['words'])
  end
end
