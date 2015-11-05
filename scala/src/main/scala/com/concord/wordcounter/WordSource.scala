package com.concord.wordcounter

import java.util.{ HashSet => MutableHashSet}

import com.concord._
import com.concord.swift._

import scala.util.Random


class WordSource extends Computation {
  private val words = Array("foo", "bar", "baz", "fiz", "buzz")
  private val rand = new Random()

  def sample(): String = words( rand.nextInt(words.length) )

  override def init(ctx: ComputationContext): Unit = {
    println(s"${this.getClass.getSimpleName} initialized")
    ctx.setTimer("loop", System.currentTimeMillis())
  }

  override def processRecord(ctx: ComputationContext, record: Record): Unit = ???

  override def processTimer(ctx: ComputationContext, key: String, time: Long): Unit = {
    // Stream, key, value. Empty value, no need for val
    Range(0, 1024).foreach {
      i => ctx.produceRecord("words".getBytes, sample().getBytes, "-".getBytes)
    }

    ctx.setTimer(key, System.currentTimeMillis() + 5000)
  }

  override def metadata(): Metadata = {
    val ostreams = new MutableHashSet[String](java.util.Arrays.asList("words"))
    new Metadata("word-source", new MutableHashSet[StreamTuple](), ostreams)
  }

}


object WordSource {
  def main(args: Array[String]): Unit = {
    ServeComputation.serve(new WordSource())
  }
}
