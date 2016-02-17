package io.concord.wordcounter;

import io.concord.*;
import io.concord.swift.*;
import java.util.*;

public class SentenceGenerator extends Computation {
  private final String[] howDoILoveTheeLines = { 
    "How do I love thee? Let me count the ways.",
    "I love thee to the depth and breadth and height",
    "My soul can reach, when feeling out of sight",
    "For the ends of being and ideal grace.",
    "I love thee to the level of every day's",
    "Most quiet need, by sun and candle-light.",
    "I love thee freely, as men strive for right.",
    "I love thee purely, as they turn from praise.",
    "I love thee with the passion put to use",
    "In my old griefs, and with my childhood's faith.",
    "I love thee with a love I seemed to lose",
    "With my lost saints. I love thee with the breath,",
    "Smiles, tears, of all my life; and, if God choose,",
    "I shall but love thee better after death."
  };

  private final Random rand = new Random();
  private String sample() {
    int idx = rand.nextInt(howDoILoveTheeLines.length);
    return howDoILoveTheeLines[idx];
  }

  public void init(ComputationContext ctx) {
    System.out.println("SentenceGenerator.java initialized");
    ctx.setTimer("loop", System.currentTimeMillis());
  }

  public void processRecord(ComputationContext ctx, Record record) {
    throw new RuntimeException("Method not implemented");
  }

  public void processTimer(ComputationContext ctx, String key, long time) {
    // Stream, key, value. Empty value, no need for val
    for (int i = 0; i < 10000; ++i) {
      ctx.produceRecord("sentences".getBytes(), this.sample().getBytes(),
                        "-".getBytes());
    }

    ctx.setTimer(key, System.currentTimeMillis());
  }

  public Metadata metadata() {
    return new Metadata("sentence-generator", new HashSet<StreamTuple>(),
                        new HashSet<String>(Arrays.asList("sentences")));
  }

  public static void main(String[] args) {
    ServeComputation.serve(new SentenceGenerator());
  }
}
