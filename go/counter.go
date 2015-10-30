package main

import (
	"github.com/concord/concord-go"
	"log"
	"strconv"
	"sync"
	"time"
)

type Counter struct {
	mu    sync.Mutex // owns map below
	stats map[string]int64
}

func (c *Counter) Init(ctx *concord.Context) error {
	log.Println("[INFO] initialize word's counter")
	c.stats = make(map[string]int64)

	go func() {
		for {
			c.mu.Lock()
			log.Println(c.stats)
			c.mu.Unlock()
			time.Sleep(5 * time.Second)
		}
	}()

	return nil
}

func (c *Counter) ProcessTimer(*concord.Context, int64, string) error {
	return nil
}

func (c *Counter) ProcessRecords(ctx *concord.Context, record *concord.Record) error {
	c.mu.Lock()
	defer c.mu.Unlock()

	k := string(record.Key)
	if v, err := strconv.Atoi(string(record.Data)); err == nil {
		c.stats[k] += int64(v)
	}
	return nil
}

func (c *Counter) Metadata() *concord.Metadata {
	return &concord.Metadata{
		Name:   "counter",
		Inputs: []*concord.Stream{concord.NewDefaultStream("words")},
	}
}
