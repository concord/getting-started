package main

import (
	"github.com/concord/concord-go"
	"log"
	"math/rand"
	"strconv"
	"time"
)

type Producer struct {
	words []string
}

func (p *Producer) Init(ctx *concord.Context) error {
	log.Println("[INFO] initialize word's producer")
	rand.Seed(time.Now().UnixNano())
	p.words = append(p.words, "foo", "bar", "baz", "buzz")
	ctx.SetTimer(time.Now(), "")
	return nil
}

func (p *Producer) ProcessTimer(ctx *concord.Context, ts int64, name string) error {
	for _, w := range p.words {
		n := strconv.Itoa(rand.Intn(100))
		ctx.ProduceRecord("words", w, n)
	}
	ctx.SetTimer(time.Now().Add(5*time.Second), "")
	return nil
}

func (p *Producer) ProcessRecords(*concord.Context, *concord.Record) error {
	return nil
}

func (p *Producer) Metadata() *concord.Metadata {
	return &concord.Metadata{
		Name:    "word-producer",
		Outputs: []string{"words"},
	}
}
