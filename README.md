<img src="misc/concord-light-full.png" alt="Concord logo" height="135" />

# Concord Stream Processor

####A modern and performant distributed stream processing framework

Concord is a native stream processing framework (implemented in C++) built on Apache Mesos,
built to address some of the following issues commonly encountered when creating streaming
jobs:

- Stream processing shouldn’t be restricted to distributed systems experts -- application
programmers and data scientists should be able to write streaming computations in the languages
they’re comfortable with
- Cluster management should be accessible for regular developers, not just specialists
- Debugging a distributed system (& tracking down errors) should be simple
- For multi-tenancy, processes must be isolated from and protected against one another


## Getting Started

Concord depends on a number of runtime dependencies in order to start, launch and run computations.
To make it easy to test drive Concord, we have setup a vagrant environment that includes everything 
you'll need to create and launch your own computations in at least 5 different programming languages.

Just run:
```
./bootstrap_vagrant.sh && vagrant ssh
```
**[and follow the instructions here](http://docs.concord.io/quick_start_ruby.html).**
