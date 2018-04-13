#pragma once

#include <map>
#include <set>
#include <vector>

template <typename T>
size_t getIndexOf(std::vector<T> const& vec, T const& element) {
  size_t result;
  for (result = 0; result < vec.size(); ++result)
    if (vec.at(result) == element) return result;
  return result;
}

template <typename T>
bool contain(std::vector<T> const& vec, T const& element) {
  return getIndexOf(vec, element) < vec.size();
}

template <typename T>
void removeElement(std::vector<T> const& vec, T const& element) {
  auto index = getIndexOf(vec, element);
  if (index >= vec.size()) return;
  for (; index + 1 < vec.size(); ++index) vec.at(index) = vec.at(index + 1);
  vec.pop_back();
}

template <typename>
class Node;

template <typename DATA_TYPE>
class Graph;

using NodeID = size_t;

template <typename DATA_TYPE>
class Node {
  public:
  DATA_TYPE const& getData() const;
  void setData(DATA_TYPE const& d);

  size_t nofChilds() const;
  NodeID getChild(size_t i) const;
  size_t getChildIndex(NodeID const& node) const;
  void addChild(NodeID const& node);
  void removeChild(NodeID const& node);
  bool hasChild(NodeID const& node) const;

  size_t nofParents() const;
  NodeID getParent(size_t i) const;
  size_t getParentIndex(NodeID const& node) const;
  void addParent(NodeID parent);
  void removeParent(NodeID i);
  bool hasParent(NodeID const& node) const;

  protected:
  Node(Graph<DATA_TYPE>* graph);
  DATA_TYPE           data;
  Graph<DATA_TYPE>*   graph;
  std::vector<NodeID> childs;
  std::vector<NodeID> parents;
};

template <typename DATA_TYPE>
class Graph {
  public:
  ~Graph();
  void addRoot(NodeID const& id);
  void removeRoot(NodeID const& id);
  size_t nofRoots() const;
  NodeID getRoot(size_t i) const;
  bool isRoot(NodeID const& id) const;
  size_t getRootIndex(NodeID const& root) const;
  NodeID createNode();
  void deleteNode(NodeID const& id);
  bool hasNode(NodeID const& id) const;
  Node<DATA_TYPE>* getNode(NodeID const& id);
  NodeID getNodeID(Node<DATA_TYPE> const* const& node) const;

  protected:
  std::vector<NodeID> roots;
  std::map<NodeID, Node<DATA_TYPE>*> id2Node;
  std::map<Node<DATA_TYPE>*, NodeID> node2Id;
  std::set<NodeID> freeIds;
  size_t           idCounter = 0;
  NodeID           reserveFreeID();
};

template <typename DATA_TYPE>
DATA_TYPE const& Node<DATA_TYPE>::getData() const {
  return data;
}

template <typename DATA_TYPE>
void Node<DATA_TYPE>::setData(DATA_TYPE const& d) {
  data = d;
}

template <typename DATA_TYPE>
size_t Node<DATA_TYPE>::nofChilds() const {
  return childs.size();
}

template <typename DATA_TYPE>
NodeID Node<DATA_TYPE>::getChild(size_t i) const {
  if (i >= nofChilds()) throw std::out_of_range("Cannot get child");
  return childs.at(i);
}

template <typename DATA_TYPE>
size_t Node<DATA_TYPE>::getChildIndex(NodeID const& node) const {
  if (!graph->hasNode(node))
    throw std::invalid_argument("Cannot get child index - node does not exist");
  return getIndexOf(childs, node);
}

template <typename DATA_TYPE>
void Node<DATA_TYPE>::addChild(NodeID const& id) {
  if (hasChild(id)) return;
  childs.push_back(id);
  graph->getNode(id)->addParent(this);
}

template <typename DATA_TYPE>
void Node<DATA_TYPE>::removeChild(NodeID const& id) {
  if (!hasChild(id)) return;
  removeElement(childs, id);
  graph->getNode(id)->removeParent(graph->getNodeID(this));
}

template <typename DATA_TYPE>
bool Node<DATA_TYPE>::hasChild(NodeID const& id) const {
  return contain(childs, id);
}

template <typename DATA_TYPE>
size_t Node<DATA_TYPE>::nofParents() const {
  return parents.size();
}

template <typename DATA_TYPE>
NodeID Node<DATA_TYPE>::getParent(size_t i) const {
  if (i >= nofParents())
    throw std::out_of_range("Node::getParent() - Cannot get parent");
  return parents.at(i);
}

template <typename DATA_TYPE>
void Node<DATA_TYPE>::addParent(NodeID parent) {
  if (hasParent(parent)) return;
  parents.push_back(parent);
  graph->getNode(parent)->addChild(this);
}

template <typename DATA_TYPE>
size_t Node<DATA_TYPE>::getParentIndex(NodeID const& node) const {
  return getIndexOf(parents, node);
}

template <typename DATA_TYPE>
void Node<DATA_TYPE>::removeParent(NodeID i) {
  if (!hasParent(i)) return;
  removeElement(parents, i);
  graph->getNode(i)->removeChild(graph->getNodeID(this));
}

template <typename DATA_TYPE>
bool Node<DATA_TYPE>::hasParent(NodeID const& id) const {
  return contain(parents, id);
}

template <typename DATA_TYPE>
Graph<DATA_TYPE>::~Graph() {
  for (auto const& n : id2Node) delete n->second;
}

template <typename DATA_TYPE>
void Graph<DATA_TYPE>::addRoot(NodeID const& id) {
  if (!hasNode(id))
    throw std::invalid_argument("Cannot add root node - it does not exist");
  if (isRoot(id)) return;
  roots.push_back(id);
}

template <typename DATA_TYPE>
void Graph<DATA_TYPE>::removeRoot(NodeID const& id) {
  if (!isRoot(id)) return;
  size_t rootIndex = getRootIndex(id);
  for (; rootIndex + 1 < nofRoots(); ++rootIndex)
    roots.at(rootIndex) = roots.at(rootIndex + 1);
  roots.pop_back();
}

template <typename DATA_TYPE>
size_t Graph<DATA_TYPE>::nofRoots() const {
  return roots.size();
}

template <typename DATA_TYPE>
NodeID Graph<DATA_TYPE>::getRoot(size_t i) const {
  if (i >= nofRoots()) throw std::out_of_range("Cannot get root");
  return roots.at(i);
}

template <typename DATA_TYPE>
bool Graph<DATA_TYPE>::isRoot(NodeID const& id) const {
  for (auto const& r : roots)
    if (r == id) return true;
  return false;
}

template <typename DATA_TYPE>
size_t Graph<DATA_TYPE>::getRootIndex(NodeID const& root) const {
  if (!isRoot(root))
    throw std::invalid_argument("Cannot get root index - node is not root");
  return getIndexOf(roots, root);
}

template <typename DATA_TYPE>
NodeID Graph<DATA_TYPE>::createNode() {
  auto const id   = reserveFreeID();
  auto       node = new Node<DATA_TYPE>(this);
  id2Node[id]     = node;
  node2Id[node]   = id;
  return id;
}

template <typename DATA_TYPE>
void Graph<DATA_TYPE>::deleteNode(NodeID const& id) {
  auto node = getNode(id);
  for (auto const& p : node->parents) p->removeChild(id);
  for (auto const& c : node->childs) c->removeParent(id);
  if (isRoot(id)) removeRoot(id);
  id2Node.erase(id);
  node2Id.erase(node);
  delete node;
  freeIds.insert(id);
}

template <typename DATA_TYPE>
bool Graph<DATA_TYPE>::hasNode(NodeID const& id) const {
  return id2Node.count(id) > 0;
}

template <typename DATA_TYPE>
Node<DATA_TYPE>* Graph<DATA_TYPE>::getNode(NodeID const& id) {
  auto it = id2Node.find(id);
  if (it == id2Node.end()) throw std::invalid_argument("there is no such node");
  return it->second;
}

template <typename DATA_TYPE>
NodeID Graph<DATA_TYPE>::getNodeID(Node<DATA_TYPE> const* const& node) const {
  auto it = node2Id.find(node);
  if (it == node2Id.end()) throw std::invalid_argument("there is no such node");
  return it->second;
}

template <typename DATA_TYPE>
NodeID Graph<DATA_TYPE>::reserveFreeID() {
  if (freeIds.empty()) return idCounter++;
  auto id = *freeIds.begin();
  freeIds.erase(id);
  return id;
}
