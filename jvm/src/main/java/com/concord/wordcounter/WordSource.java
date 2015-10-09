package com.concord;

import com.concord.*;
import com.concord.swift.*;
import java.util.*;

public class WordSource extends Computation {
  private final String[] words = {"foo", "bar", "baz", "fiz", "buzz"};
  private final Random rand = new Random();
  private String sample() {
    int idx = rand.nextInt(words.length);
    return words[idx];
  }

  public void init(ComputationContext ctx) {
    System.out.println("WordSource.java initialized");
    ctx.setTimer("loop", System.currentTimeMillis());
  }

  public void processRecord(ComputationContext ctx, Record record) {
    throw new RuntimeException("Method not implemented");
  }

  public void processTimer(ComputationContext ctx, String key, long time) {
    // Stream, key, value. Empty value, no need for val
    for (int i = 0; i < 1024; ++i) {
      ctx.produceRecord("words".getBytes(), this.sample().getBytes(),
                        "-".getBytes());
    }

    ctx.setTimer(key, System.currentTimeMillis() + 5000);
  }

  public Metadata metadata() {
    return new Metadata("word-source", new HashSet<StreamTuple>(),
                        new HashSet<String>(Arrays.asList("words")));
  }

  public static void main(String[] args) {
    ServeComputation.serve(new WordSource());
  }
}
