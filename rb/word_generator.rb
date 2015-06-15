require 'concord'

DICTIONARY = ['foo', 'bar', 'baz', 'qux']

def log(str)
  $stderr.puts str
  $stderr.flush
end


def time_millis()
  (Time.now.to_f * 1000.0).to_i
end

class WordGenerator
  def init(ctx)
    future = time_millis
    log "Initialized Computation"
    ctx.set_timer('default', time_millis)
  end

  def process_timer(ctx, key, time)
    (0..1024).each do |i|
      ctx.produce_record('words', DICTIONARY.sample, '')
    end
    ctx.set_timer(key, time_millis + 5000)
  end

  def metadata
    log "Metadata called"
    Concord::Metadata.new(name: 'word-generator', ostreams: ['words'])
  end
end

Concord::Computation.serve(WordGenerator.new)
