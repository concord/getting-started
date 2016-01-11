package io.concord.wordcounter

import java.util.{ HashSet => MutableHashSet}

import io.concord._
import io.concord.swift._

import scala.util.Random


class SentenceGenerator extends Computation {
  private val rand = new Random()
  private val sentences = Array("How do I love thee? Let me count the ways.",
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
    "I shall but love thee better after death.")

  def sample(): String = sentences( rand.nextInt(sentences.length) )

  override def init(ctx: ComputationContext): Unit = {
    println(s"${this.getClass.getSimpleName} initialized")
    ctx.setTimer("loop", System.currentTimeMillis())
  }

  override def processRecord(ctx: ComputationContext, record: Record): Unit = ???

  override def processTimer(ctx: ComputationContext, key: String, time: Long): Unit = {
    // Stream, key, value. Empty value, no need for val
    Range(0, 1024).foreach {
      i => ctx.produceRecord("sentences".getBytes, sample().getBytes, "-".getBytes)
    }

    ctx.setTimer(key, System.currentTimeMillis() + 5000)
  }

  override def metadata(): Metadata = {
    val ostreams = new MutableHashSet[String](java.util.Arrays.asList("sentences"))
    new Metadata("sentence-generator", new MutableHashSet[StreamTuple](), ostreams)
  }

}


object SentenceGenerator {
  def main(args: Array[String]): Unit = {
    ServeComputation.serve(new SentenceGenerator())
  }
}
