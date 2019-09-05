#include "gtest/gtest.h"
#include "trace/span-context.h"

namespace {
TEST(SpanContext, Parentless) {
  auto ctx = new SpanContext(nullptr);
  EXPECT_EQ(sizeof(ctx->get_trace_id()), 16);
  EXPECT_EQ(sizeof(ctx->get_span_id()), 8);
}
TEST(SpanContext, Parentful) {
  auto ctx = new SpanContext(nullptr);
  auto child = new SpanContext(ctx);
  EXPECT_EQ(child->get_trace_id(), ctx->get_trace_id());
  EXPECT_EQ(sizeof(ctx->get_span_id()), 8);
}
}
