#include <StringAligner/Cell.h>
#include <StringAligner/Table.h>
#include <algorithm>
#include <numeric>
#include <sstream>

using namespace stringAligner;
using namespace std;

void insertToVectorAndResizeIfNecessary(vector<string>& v,
                                        size_t          i,
                                        string const&   d)
{
  if (i >= v.size()) v.resize(i + 1, "");
  v.at(i) = d;
}

void Table::setVerticalDecorator(size_t column, string const& decorator)
{
  needToRecomputeWidth = true;
  insertToVectorAndResizeIfNecessary(verticalDecorators, column, decorator);
}

void Table::setHorizontalDecorator(size_t row, string const& decorator)
{
  needToRecomputeHeight = true;
  insertToVectorAndResizeIfNecessary(horizonalDecorators, row, decorator);
}

void Table::addRow()
{
  auto const nofColumns = getNofColumns();
  cells.push_back(vector<Cell>());
  cells.back().resize(nofColumns);
  rowHeights.push_back(0);
  needToRecomputeHeight = true;
}

void Table::addColumn()
{
  for (auto& r : cells) r.push_back({});
  columnWidths.push_back(0);
  needToRecomputeWidth = true;
}

size_t Table::getNofColumns() const
{
  if (cells.empty()) return 0;
  return cells.at(0).size();
}

size_t Table::getNofRows() const { return cells.size(); }

void throwIfColumnIsTooLarge(size_t nofColumns, size_t i)
{
  if (i < nofColumns) return;
  stringstream ss;
  ss << "cannot call stringAligner::Table::getColumnWidth(" << i << ") - ";
  ss << "column index: " << i
     << " is larger than number of columns: " << nofColumns;
  throw runtime_error(ss.str());
}

size_t Table::getColumnWidth(size_t i) const
{
  throwIfColumnIsTooLarge(getNofColumns(), i);
  return columnWidths.at(i);
}

void throwIfRowIsTooLarge(size_t nofRows, size_t i)
{
  if (i < nofRows) return;
  stringstream ss;
  ss << "cannot call stringAligner::Table::getRowHeight(" << i << ") - ";
  ss << "row index: " << i << " is larger than number of rows: " << nofRows;
  throw runtime_error(ss.str());
}

size_t Table::getRowHeight(size_t i) const
{
  throwIfRowIsTooLarge(getNofRows(), i);
  return rowHeights.at(i);
}

bool Table::isDecoratorThenConvertIndexToRowIndexAndDecoratorIndex(
    size_t& row,
    size_t& decorator,
    size_t& i) const
{
  decorator = 0;
  row       = 0;

  if (i == 0 && getVerticalDecorator(0) != "") return true;

  i -= getVerticalDecorator(0).size();
  while (i >= getRowHeight(row)) {
    i -= getRowHeight(row);
    if (i == 0 && getVerticalDecorator(row + 1).size() != 0) {
      decorator = row + 1;
      return true;
    }
    i -= getVerticalDecorator(row + 1).size();
    ++row;
  }
  return false;
}

Cell const& Table::getCellInternal(size_t row, size_t column) const
{
  return cells.at(row).at(column);
}

string Table::getRowLine(size_t rowId, size_t line) const
{
  stringstream ss;
  ss << getHorizontalDecorator(0);
  for (size_t c = 0; c < getNofColumns(); ++c) {
    auto const& cell = getCellInternal(rowId, c);
    ss << cell.getLine(getColumnWidth(c), getRowHeight(rowId), line);
    ss << getHorizontalDecorator(c + 1);
  }
  return ss.str();
}

void throwIfLineIsOutOfRange(size_t height, size_t i)
{
  if (i < height) return;
  stringstream ss;
  ss << "cannot call: stringstream::Table::getLine"
     << "(" << i << ") - ";
  ss << "line: " << i << " is larger than number of lines: " << height;
  throw runtime_error(ss.str());
}

string Table::getLine(size_t line) const
{
  throwIfLineIsOutOfRange(getHeight(), line);

  size_t row, decoratorId;
  if (isDecoratorThenConvertIndexToRowIndexAndDecoratorIndex(row, decoratorId,
                                                             line))
    return string(getWidth(), getVerticalDecorator(decoratorId).at(0));

  return getRowLine(row, line);
}

string Table::getData() const
{
  stringstream ss;
  for (size_t i = 0; i < getHeight(); ++i) ss << getLine(i) << endl;
  return ss.str();
}

string returnDefaultIfOutOfRange(vector<string> const& v, size_t i)
{
  if (i >= v.size()) return "";
  return v.at(i);
}

string Table::getVerticalDecorator(size_t i) const
{
  return returnDefaultIfOutOfRange(verticalDecorators, i);
}

string Table::getHorizontalDecorator(size_t i) const
{
  return returnDefaultIfOutOfRange(horizonalDecorators, i);
}

size_t computeDecoratorsLength(
    std::function<string(size_t)> const& getDecorator,
    size_t                               n)
{
  size_t counter = getDecorator(0).size();
  for (size_t i = 0; i < n; ++i) counter += getDecorator(i + 1).size();
  return counter;
}

size_t computeLength(std::vector<size_t> const&           sizes,
                     std::function<string(size_t)> const& getDecorator)
{
  size_t counter = 0;
  counter += accumulate(sizes.begin(), sizes.end(), 0);
  counter += computeDecoratorsLength(getDecorator, sizes.size());
  return counter;
}

size_t Table::getWidth() const
{
  if (needToRecomputeWidth) {
    width                = computeLength(columnWidths,
                          [&](size_t i) { return getHorizontalDecorator(i); });
    needToRecomputeWidth = false;
  }
  return width;
}

size_t Table::getHeight() const
{
  if (needToRecomputeHeight) {
    height                = computeLength(rowHeights,
                           [&](size_t i) { return getVerticalDecorator(i); });
    needToRecomputeHeight = false;
  }
  return height;
}

template <typename T>
void replaceVector(vector<T>& output, vector<T> const& input)
{
  if (output.size() < input.size()) output.resize(input.size());
  copy(input.cbegin(), input.cend(), output.begin());
}

void Table::setVerticalDecorators(vector<string> const& d)
{
  needToRecomputeHeight = true;
  replaceVector(verticalDecorators, d);
}

void Table::setHorizontalDecorators(vector<string> const& d)
{
  needToRecomputeWidth = true;
  replaceVector(horizonalDecorators, d);
}

void throwIfRowIsOutOfRange(size_t        nofRows,
                            size_t        row,
                            size_t        column,
                            string const& fce)
{
  if (row < nofRows) return;
  stringstream ss;
  ss << "cannot call stringAligner::Table::" << fce << "(" << row << ","
     << column << ")";
  ss << " - row = " << row << " >= nofRows = " << nofRows;
  throw runtime_error(ss.str());
}

void throwIfColumnIsOutOfRange(size_t        nofColumns,
                               size_t        row,
                               size_t        column,
                               string const& fce)
{
  if (column < nofColumns) return;
  stringstream ss;
  ss << "cannot call stringAligner::Table::" << fce << "(" << row << ","
     << column << ")";
  ss << " - column = " << column << " >= nofColumns = " << nofColumns;
  throw runtime_error(ss.str());
}

void insertIfLargerAndResizeIfNeeded(std::vector<size_t>& v, size_t i, size_t d)
{
  if (v.size() <= i) v.resize(i + 1, 0);
  v.at(i) = std::max(v.at(i), d);
}

size_t computeRowHeight(std::vector<std::vector<Cell>> const& cells, size_t row)
{
  size_t result = 0;
  for (auto const& c : cells.at(row)) result = std::max(result, c.getHeight());
  return result;
}

size_t computeColumnWidth(std::vector<std::vector<Cell>> const& cells,
                          size_t                                column)
{
  size_t result = 0;
  for (auto const& r : cells)
    result = std::max(result, r.at(column).getWidth());
  return result;
}

void clearOldBlockOnChangeCallback(Cell& cell)
{
  auto oldBlock = cell.getData();
  if (oldBlock) oldBlock->setOnChange(nullptr);
}

void Table::setOnChangeCallback(shared_ptr<Block> const& block,
                                size_t                   row,
                                size_t                   column)
{
  block->setOnChange([row, column, this](Block* b) {
    rowHeights.at(row)      = std::max(rowHeights.at(row), b->getHeight());
    columnWidths.at(column) = std::max(columnWidths.at(column), b->getWidth());
    needToRecomputeWidth    = true;
    needToRecomputeHeight   = true;
  });
}

void Table::setCell(size_t row, size_t column, shared_ptr<Block> const& block)
{
  throwIfRowIsOutOfRange(getNofRows(), row, column, __FUNCTION__);
  throwIfColumnIsOutOfRange(getNofColumns(), row, column, __FUNCTION__);

  auto& cell = cells.at(row).at(column);

  clearOldBlockOnChangeCallback(cell);

  cell.setData(block);

  if (!block) {
    rowHeights.at(row)      = computeRowHeight(cells, row);
    columnWidths.at(column) = computeColumnWidth(cells, column);
    needToRecomputeWidth    = true;
    needToRecomputeHeight   = true;
    return;
  }

  insertIfLargerAndResizeIfNeeded(rowHeights, row, block->getHeight());
  insertIfLargerAndResizeIfNeeded(columnWidths, column, block->getWidth());

  setOnChangeCallback(block, row, column);
}

std::shared_ptr<Block> Table::getCell(size_t row, size_t column) const
{
  throwIfRowIsOutOfRange(getNofRows(), row, column, __FUNCTION__);
  throwIfColumnIsOutOfRange(getNofColumns(), row, column, __FUNCTION__);
  auto const& cell = cells.at(row).at(column);
  return cell.getData();
}
