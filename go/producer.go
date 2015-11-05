package main

import (
	"github.com/concord/concord-go"
	"log"
	"math/rand"
	"strconv"
	"time"
)

// Producer holds computation state
type Producer struct {
	words []string
}

// Metadata is called by Concord to define input/output streams for computation
// This computation only writes data to stream "words"
func (p *Producer) Metadata() *concord.Metadata {
	return &concord.Metadata{
		Name:    "word-producer",
		Outputs: []string{"words"},
	}
}

// Initialization of computation. This is method is called by Concord
func (p *Producer) Init(ctx *concord.Context) error {
	log.Println("[INFO] initialize word's producer")
	rand.Seed(time.Now().UnixNano())
	p.words = append(p.words, "foo", "bar", "baz", "buzz")

	// schedule next ProcessTimer call
	ctx.SetTimer(time.Now(), "")

	return nil
}

// ProcessTimer is called by Concord at the time specified by SetTimer call.
func (p *Producer) ProcessTimer(ctx *concord.Context, ts int64, name string) error {
	for _, w := range p.words {
		n := strconv.Itoa(rand.Intn(100))
		ctx.ProduceRecord("words", w, n)
	}

	// schedule next ProcessTimer invokation
	ctx.SetTimer(time.Now().Add(5*time.Second), "")

	return nil
}

func (p *Producer) ProcessRecords(*concord.Context, *concord.Record) error {
	return nil
}
