require 'concord'
require 'concord/utils'

class SentenceSplitter
  def init(context)
    Concord::Utils.log_to_stderr("Initialized Splitter computation")
  end

  # Use the destroy method to perform some cleanup before the framework
  # kills this process
  def destroy
    Concord::Utils.log_to_stderr("Destructing splitter computation")
  end

  def process_record(context, record)
    record.key.split do |word|
      context.produce_record('words', word, '-')
    end
  end

  def metadata
    Concord::Metadata.new(name: 'sentence-splitter',
                          istreams: [['sentences', Concord::Thrift::StreamGrouping::SHUFFLE]],
                          ostreams: ['words'])
  end
end

Concord::Computation.serve(SentenceSplitter.new)
