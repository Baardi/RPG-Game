#pragma once
#include <vector>

template <typename T>
class Node
{
  const T &get() const { return m_content; }
  T &get() { return m_content; }

  const std::vector<Node<T>> &children() const { return m_children; }
  std::vector<Node<T>> &children() { return m_children; }

  template <typename F>
  void foreachRecursive(F func)
  {
    func(m_content);
    for (auto &child : m_children)
      child.foreachRecursive(func);
  }

  template <typename F>
  void foreachRecursive(F func) const
  {
    func(m_content);
    for (auto &child : m_children)
      child.foreachRecursive(func);
  }

private:
  T m_content;
  std::vector<Node<T>> m_children;
};