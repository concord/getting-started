package main

import (
	"github.com/concord/concord-go"
	"log"
	"math/rand"
	"time"
)

// SentenceGenerator holds computation state
type SentenceGenerator struct { }

const (
	producerTimer = "main"
)

var (
	// Arrays cannot be constant
	sentences  = [...]string { "lmao", "rofl" }
)

// Metadata is called by Concord to define input/output streams for computation
// This computation only writes data to stream "words"
func (p *SentenceGenerator) Metadata() *concord.Metadata {
	return &concord.Metadata{
		Name:    "sentence-generator",
		Outputs: []string{"sentences"},
	}
}

// Initialization of computation. This is method is called by Concord
func (p *SentenceGenerator) Init(ctx *concord.Context) error {
	log.Println("[INFO] Initializing SentenceGenerator")
	rand.Seed(time.Now().UnixNano())

	// schedule next ProcessTimer call
	ctx.SetTimer(producerTimer, time.Now())

	return nil
}

// ProcessTimer is called by Concord at the time specified by SetTimer call.
func (p *SentenceGenerator) ProcessTimer(ctx *concord.Context, ts int64, name string) error {
	for _, sentence := range sentences {
		ctx.ProduceRecord("words", sentence, "-")
	}

	// schedule next ProcessTimer invocation
	ctx.SetTimer(producerTimer, time.Now().Add(5*time.Second))

	return nil
}

func (p *SentenceGenerator) ProcessRecords(*concord.Context, *concord.Record) error {
	return nil
}


func main() {
	var generator concord.Computation = &SentenceGenerator{}
	log.Fatal(concord.Serve(generator))
}
