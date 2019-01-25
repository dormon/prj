#include <StringAligner/Text.h>
#include <sstream>

using namespace stringAligner;
using namespace std;

void Text::addLine() { lines.push_back(""); }

Text::Text(string const &txt, size_t m, char f) : maxWidth(m), filler(f)
{
  if (txt == "") return;
  addLine();
  addText(txt);
}

void Text::clear(size_t maxLen, char fil)
{
  lines.clear();
  maxWidth = maxLen;
  filler   = fil;
  callOnChange();
}

void Text::addTextRec(std::string const &txt)
{
  auto &line = lines.back();
  if (line.length() + txt.length() <= maxWidth) {
    line += txt;
    return;
  }
  size_t const headLength = maxWidth - line.length();
  auto const   head       = txt.substr(0, headLength);
  auto const   tail       = txt.substr(headLength, txt.length() - headLength);
  line += head;
  addLine();
  addTextRec(tail);
}

void Text::addText(string const &txt)
{
  if (lines.empty()) addLine();
  addTextRec(txt);
  callOnChange();
}

void Text::newLine()
{
  addLine();
  callOnChange();
}

char Text::getFiller() const { return filler; }

size_t Text::getWidth() const { return maxWidth; }

size_t Text::getHeight() const { return lines.size(); }

string Text::getLine(size_t i) const
{
  auto const & line = lines.at(i);
  stringstream ss;
  ss << line;
  size_t const fillerLength = maxWidth - line.length();
  ss << string(fillerLength, filler);
  return ss.str();
}

string Text::getData() const
{
  stringstream ss;
  for (size_t i = 0; i < getHeight(); ++i) ss << getLine(i);
  return ss.str();
}
