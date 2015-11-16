require 'concord'
require 'concord/utils'

# Stateless computation that splits sentences into words
class SentenceSplitter
  def init(context)
    Concord::Utils.log("Initialized Sentence Splitter")
  end

  def process_record(context, record)
    # Split sentence into words..
    words = record.data.split(' ')
    words.each do |word|
      # .. send word as key to downstream operators
      context.produce_record('words', word, '')
    end
  end

  def metadata
    Concord::Utils.log("Metadata called")
    Concord::Metadata.new(name: 'sentence-splitter',
                          istreams: ['sentences'],
                          ostreams: ['words'])
  end
end

Concord::Computation.serve(SentenceSplitter.new)
