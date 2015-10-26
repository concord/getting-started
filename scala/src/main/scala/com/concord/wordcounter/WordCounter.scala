package com.concord.wordcounter

import java.util

import com.concord._
import com.concord.swift._

class WordCounter extends Computation {

  private val histogram: util.HashMap[String, Integer] = new util.HashMap()
  private var pidx = 0

  def init(ctx: ComputationContext): Unit = {
    println(s"${this.getClass.getSimpleName} initialized")
  }

  def processTimer(ctx: ComputationContext, key: String, time: Long): Unit = {
    throw new RuntimeException("Method not implemented")
  }

  def processRecord(ctx: ComputationContext, record: Record): Unit = {
    try {
      val key = new String(record.getKey, "UTF-8")
      var currentValue = histogram.get(key)
      if (currentValue != null) {
        currentValue += 1
        histogram.put(key, currentValue)
      }
      else
        histogram.put(key, 1)
    } catch {
      case e: Exception => throw new RuntimeException(e)
    }

    pidx += 1
    if (pidx % 1024 == 0) {
      println(histogram.toString)
    }
  }

  def metadata(): Metadata = {
    val istreams = new util.HashSet[StreamTuple]()
    istreams.add(new StreamTuple("words", StreamGrouping.SHUFFLE))
    new Metadata("word-counter", istreams, new util.HashSet[String]())
  }

}


object WordCounter {
  def main(args: Array[String]): Unit = {
    ServeComputation.serve(new WordCounter())
  }
}
