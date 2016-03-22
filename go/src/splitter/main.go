package main

import (
	"github.com/concord/concord-go"
	"log"
	"strings"
)

type SentenceSplitter struct { }

func (c *SentenceSplitter) Metadata() *concord.Metadata {
	return &concord.Metadata{
		Name:   "sentence-splitter",
		Inputs: []*concord.Stream{concord.NewDefaultStream("sentences")},
		Outputs: []string{"words"},
	}
}

func (p *SentenceSplitter) Init(ctx *concord.Context) error {
	log.Println("[INFO] Initializing sentence-splitter")
	return nil
}

func (p *SentenceSplitter) Destroy() error {
	log.Println("[INFO] Destroying sentence-splitter")
	return nil
}

func (p *SentenceSplitter) ProcessTimer(ctx *concord.Context, ts int64, name string) error {
	return nil
}

func (p *SentenceSplitter) ProcessRecords(ctx *concord.Context, record *concord.Record) error {
	sentences := string(record.Key)
	words := strings.Fields(sentences)
	for _, word := range words {
		ctx.ProduceRecord("words", word, "-")
	}
	return nil
}

func main() {
	var splitter concord.Computation = &SentenceSplitter{}
	log.Fatal(concord.Serve(splitter))
}

	
