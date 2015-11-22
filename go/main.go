package main

import (
	"flag"
	"github.com/concord/concord-go"
	"log"
	"os"
)

var (
	producer = flag.Bool("producer", false, "act as words producer")
)

func init() {
	log.SetOutput(os.Stdout)
}

func main() {
	flag.Parse()

	var comp concord.Computation
	if *producer {
		comp = &Producer{}
	} else {
		comp = &Counter{}
	}

	log.Fatal(concord.Serve(comp))
}
