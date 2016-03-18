package io.concord.wordcounter;

import io.concord.*;
import io.concord.swift.*;
import java.util.*;

public class SentenceSplitter extends Computation {
  
  public void init(ComputationContext ctx) {
    System.out.println("SentenceSplitter.java initialized");
  }

  public void destroy() {
    System.out.println("SentenceSplitter.java destructing");
  }

  public void processTimer(ComputationContext ctx, String key, long time) {
    throw new RuntimeException("Method not implemented");
  }
  
  public void processRecord(ComputationContext ctx, Record record) {
    try {
      String sentences = new String(record.getKey(), "UTF-8");    
      String words[] = sentences.split(" ");
      for (int i = 0; i < words.length; ++i) {
	ctx.produceRecord("words".getBytes(), words[i].getBytes(), "-".getBytes());
      }
    } catch(Exception e) {
      throw new RuntimeException(e);
    }
  }

  public Metadata metadata() {
    Set<StreamTuple> istreams = new HashSet<StreamTuple>();
    istreams.add(new StreamTuple("sentences", StreamGrouping.SHUFFLE));
    return new Metadata("sentence-splitter",
			istreams,
			new HashSet<String>(Arrays.asList("words")));
  }
  
  public static void main(String[] args) {
    ServeComputation.serve(new SentenceSplitter());
  }

}
