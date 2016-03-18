package io.concord.wordcounter

import java.util.{ HashSet => MutableHashSet }

import io.concord._
import io.concord.swift._

class SentenceSplitter extends Computation {
  override def init(ctx: ComputationContext): Unit = {
    println(s"${this.getClass.getSimpleName} initialzed")
  }

  override def destroy(): Unit = {
    println(s"${this.getClass.getSimpleName} destructing")
  }

  override def processRecord(ctx: ComputationContext, record: Record): Unit = {
    val sentences = new String(record.getKey, "UTF-8")
    val words = sentences.split(" ")
    words.foreach((word) => {
      ctx.produceRecord("words".getBytes, word.getBytes, "-".getBytes)
    })
  }

  override def processTimer(ctx: ComputationContext, key: String, time: Long): Unit = ???

  override def metadata(): Metadata = {
    val ostreams = new MutableHashSet[String](java.util.Arrays.asList("words"))
    val istreams = new MutableHashSet[StreamTuple]()
    istreams.add(new StreamTuple("sentences", StreamGrouping.SHUFFLE))
    new Metadata("sentence-splitter", istreams, ostreams)
  }

}

object SentenceSplitter {
  def main(args: Array[String]): Unit = {
    ServeComputation.serve(new SentenceSplitter())
  }
}
