package main

import (
	"github.com/concord/concord-go"
	"log"
	"strconv"
	"time"
)

// Counter holds computation state
type Counter struct {
	stats map[string]int64
}

// Metadata is called by Concord to define input/output streams for computation
// This computation only reads data from "words" stream
func (c *Counter) Metadata() *concord.Metadata {
	return &concord.Metadata{
		Name:   "counter",
		Inputs: []*concord.Stream{concord.NewDefaultStream("words")},
	}
}

// Initialization of computation. This method is called by Concord
func (c *Counter) Init(ctx *concord.Context) error {
	log.Println("[INFO] initialize word's counter")
	c.stats = make(map[string]int64)

	// schedule next ProcessTimer call
	ctx.SetTimer(time.Now(), "")

	return nil
}

// ProcessTimer is called by Concord at the time speicifed by SetTimer call.
func (c *Counter) ProcessTimer(ctx *concord.Context, t int64, name string) error {
	log.Println(c.stats)

	// schedule next ProcessTimer invokation
	ctx.SetTimer(time.Now().Add(5*time.Second), "")

	return nil
}

// ProcessRecords is called by Concord when new record arrives to "words" stream
func (c *Counter) ProcessRecords(ctx *concord.Context, record *concord.Record) error {
	k := string(record.Key)
	if v, err := strconv.Atoi(string(record.Data)); err == nil {
		c.stats[k] += int64(v)
	}
	return nil
}
