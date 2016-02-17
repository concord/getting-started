package io.concord.wordcounter

import java.util.{ HashSet => MutableHashSet}

import io.concord._
import io.concord.swift._


class WordCounter extends Computation {
  private val wordFrequencyMap = scala.collection.mutable.HashMap[String, Int]()
  private var pidx = 0

  override def init(ctx: ComputationContext): Unit = {
    println(s"${this.getClass.getSimpleName} initialized")
  }

  override def processTimer(ctx: ComputationContext, key: String, time: Long): Unit = ???

  override def processRecord(ctx: ComputationContext, record: Record): Unit = {
    val key = new String(record.getKey, "UTF-8")
    val currentValue = wordFrequencyMap.get(key)
    currentValue match {
      case Some(x: Int) => wordFrequencyMap.put(key, x + 1)
      case _ => wordFrequencyMap.put(key, 1)
    }

    pidx += 1
    if (pidx % 10000 == 0) {
      println(wordFrequencyMap)
    }
  }

  override def metadata(): Metadata = {
    val istreams = new MutableHashSet[StreamTuple]()
    istreams.add(new StreamTuple("words", StreamGrouping.SHUFFLE))
    new Metadata("word-counter", istreams, new MutableHashSet[String]())
  }

}


object WordCounter {
  def main(args: Array[String]): Unit = {
    ServeComputation.serve(new WordCounter())
  }
}
