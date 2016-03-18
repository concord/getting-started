name := "getting_started"

version := "1.0.1"

scalaVersion := "2.11.6"

scalacOptions ++= Seq("-feature", "-language:higherKinds")

libraryDependencies ++= Seq(
  "io.concord" % "concord" % "0.1.2",
  "io.concord" % "rawapi" % "0.2.5"
)


assemblyMergeStrategy in assembly := {
  case x if x.endsWith("project.clj") => MergeStrategy.discard // Leiningen build files
  case x if x.toLowerCase.startsWith("meta-inf") => MergeStrategy.discard // More bumf
  case _ => MergeStrategy.first
}


resolvers += Resolver.sonatypeRepo("public")

resolvers += "clojars" at "https://clojars.org/repo"

resolvers += "conjars" at "http://conjars.org/repo"
