package main

import (
	"github.com/concord/concord-go"
	"log"
	"strconv"
)

// WordCounter holds computation state
type WordCounter struct {
	stats map[string]int64
	count int64
}

// Metadata is called by Concord to define input/output streams for computation
// This computation only reads data from "words" stream
func (c *WordCounter) Metadata() *concord.Metadata {
	return &concord.Metadata{
		Name:   "word-counter",
		Inputs: []*concord.Stream{concord.NewDefaultStream("words")},
	}
}

// Initialization of computation. This method is called by Concord
func (c *WordCounter) Init(ctx *concord.Context) error {
	log.Println("[INFO] Initialzing WordWordCounter")
	return nil
}

// ProcessTimer is called by Concord at the time speicifed by SetTimer call.
func (c *WordCounter) ProcessTimer(ctx *concord.Context, t int64, name string) error {
	return nil
}

// ProcessRecords is called by Concord when new record arrives to "words" stream
func (c *WordCounter) ProcessRecords(ctx *concord.Context, record *concord.Record) error {
	k := string(record.Key)
	if v, err := strconv.Atoi(string(record.Data)); err == nil {
		c.stats[k] += int64(v)
	}
	if c.count % 1024 == 0 {
		log.Println("[Info] %v", c.stats)
	}
	c.count += 1
	return nil
}

func main() {
	var counter concord.Computation = &WordCounter{}
	log.Fatal(concord.Serve(counter))
}
