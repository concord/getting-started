package io.concord.wordcounter;

import io.concord.*;
import io.concord.swift.*;
import java.util.*;

public class WordCounter extends Computation {

  private final HashMap<String, Integer> histogram = new HashMap<>();
  private int pidx = 0;

  public void init(ComputationContext ctx) {
    System.out.println("WordCounter.java initialized");
  }

  public void destroy() {
    System.out.println("WordCounter.java destructing");
  }

  public void processTimer(ComputationContext ctx, String key, long time) {
    throw new RuntimeException("Method not implemented");
  }

  public void processRecord(ComputationContext ctx, Record record) {
    try {
      String key = new String(record.getKey(), "UTF-8");
      Integer currentValue = this.histogram.get(key);
      if (currentValue != null) {
        this.histogram.put(key, ++currentValue);
      } else {
        this.histogram.put(key, 1);
      }
    } catch (Exception e) {
      throw new RuntimeException(e);
    }
    if (++this.pidx % 10000 == 0) {
      System.out.println(this.histogram.toString());
    }
  }

  public Metadata metadata() {
    Set<StreamTuple> istreams = new HashSet<StreamTuple>();
    istreams.add(new StreamTuple("words", StreamGrouping.GROUP_BY));
    return new Metadata("word-counter", istreams, new HashSet<String>());
  }

  public static void main(String[] args) {
    ServeComputation.serve(new WordCounter());
  }
  
}
