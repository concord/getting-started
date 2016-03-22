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
	sentences  = [...]string {
		"How do I love thee? Let me count the ways.",
		"I love thee to the depth and breadth and height",
		"My soul can reach, when feeling out of sight",
		"For the ends of being and ideal grace.",
		"I love thee to the level of every day's",
		"Most quiet need, by sun and candle-light.",
		"I love thee freely, as men strive for right.",
		"I love thee purely, as they turn from praise.",
		"I love thee with the passion put to use",
		"In my old griefs, and with my childhood's faith.",
		"I love thee with a love I seemed to lose",
		"With my lost saints. I love thee with the breath,",
		"Smiles, tears, of all my life; and, if God choose,",
		"I shall but love thee better after death." }
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

// Destruction of computation. This is method is called by Concord
func (p *SentenceGenerator) Destroy() error {
	log.Println("[INFO] Destroying SentenceGenerator")
	return nil
}

// ProcessTimer is called by Concord at the time specified by SetTimer call.
func (p *SentenceGenerator) ProcessTimer(ctx *concord.Context, ts int64, name string) error {
	for i := 0; i < 10000; i++ {
		var sentence = sentences[rand.Intn(len(sentences))]
		ctx.ProduceRecord("sentences", sentence, "-")
	}

	// schedule next ProcessTimer invocation
	ctx.SetTimer(producerTimer, time.Now())

	return nil
}

func (p *SentenceGenerator) ProcessRecords(*concord.Context, *concord.Record) error {
	return nil
}


func main() {
	var generator concord.Computation = &SentenceGenerator{}
	log.Fatal(concord.Serve(generator))
}
