#include "trace/span.h"

std::chrono::milliseconds get_timestamp();
std::chrono::milliseconds get_timestamp() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch()
  );
}

Span::Span(
    std::string name,
    Tracer* const t,
    const SpanContext* const parent
  ) :
  tracer(t),
  name(name),
  context(new SpanContext(parent)),
  parent_context(parent),
  start_time(get_timestamp()),
  end_time(0)
{}

Span::Span(std::string name, Tracer* const t) : Span(name, t, nullptr) {}

void Span::set_name(std::string n) {
  name = n;
}

std::string Span::get_name() const {
  return name;
}

void Span::end() {
  end_time = get_timestamp();
}

void Span::end(std::chrono::milliseconds e) {
  end_time = e;
  tracer->on_span_end(this);
}

bool Span::is_ended() const {
  return end_time == std::chrono::milliseconds(0);
}

template<>
void Span::add_attribute<double>(std::string k, const double v) {
  attributes[k] = std::make_unique<const double>(v);
}

template<>
void Span::add_attribute<std::string>(std::string k, const std::string v) {
  attributes[k] = std::make_unique<const std::string>(v);
}

template<>
void Span::add_attribute<bool>(std::string k, const bool v) {
  attributes[k] = std::make_unique<const bool>(v);
}

template<>
void Span::add_attribute<int64_t>(std::string k, const int64_t v) {
  attributes[k] = std::make_unique<const int64_t>(v);
}

template<>
double Span::get_attribute<double>(std::string k) const {
  return *(std::get<std::unique_ptr<const double>>(attributes.at(k)).get());
}

template<>
std::string Span::get_attribute<std::string>(std::string k) const {
  return *(std::get<std::unique_ptr<const std::string>>(attributes.at(k)).get());
}

template<>
bool Span::get_attribute<bool>(std::string k) const {
  return *(std::get<std::unique_ptr<const bool>>(attributes.at(k)).get());
}

template<>
int64_t Span::get_attribute<int64_t>(std::string k) const {
  return *(std::get<std::unique_ptr<const int64_t>>(attributes.at(k)).get());
}

const SpanContext* Span::get_context() const {
  return context;
}

const SpanContext* Span::get_parent_context() const {
  return parent_context;
}
