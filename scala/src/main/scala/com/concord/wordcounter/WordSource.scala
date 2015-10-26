package com.concord.wordcounter

import java.util

import com.concord._
import com.concord.swift._

import scala.util.Random

class WordSource extends Computation {
  private val words = Array("foo", "bar", "baz", "fiz", "buzz")
  private val rand = new Random()

  def sample(): String = {
    val idx = rand.nextInt(words.length)
    words(idx)
  }

  def init(ctx: ComputationContext): Unit = {
    println(s"${this.getClass.getSimpleName} initialized")
    ctx.setTimer("loop", System.currentTimeMillis())
  }

  def processRecord(ctx: ComputationContext, record: Record): Unit = {
    throw new RuntimeException("Method not implemented")
  }

  def processTimer(ctx: ComputationContext, key: String, time: Long): Unit = {
    // Stream, key, value. Empty value, no need for val
    Range(0, 1024).foreach {
      i => ctx.produceRecord("words".getBytes, sample().getBytes, "-".getBytes)
    }

    ctx.setTimer(key, System.currentTimeMillis() + 5000)
  }

  def metadata(): Metadata = {
    new Metadata(
      "word-source",
      new util.HashSet[StreamTuple](),
      new util.HashSet[String](util.Arrays.asList("words"))
    )
  }

}


object WordSource {
  def main(args: Array[String]): Unit = {
    ServeComputation.serve(new WordSource())
  }
}
