#include <cassert>
#include <iostream>
#include <new>

class Union
{
protected:
  unsigned short *members;
  unsigned int _size;
public:

  Union(unsigned int size) : _size(size) {
    members = new unsigned short[_size];
    for(unsigned int i = 0; i < _size; i++) members[i] = i;
  }

  ~Union() {
    delete[] members;
  }

  void print_array() const {
    for(unsigned int i = 0; i < _size; i++) std::cout << i << ":" << members[i] << " ";
    std::cout << std::endl;
  }
  
  unsigned int get_size() const {return _size;}

  // Pure virtual functions that have to be implemented by specific union method
  virtual void unite(unsigned int member1, unsigned int member2) = 0;
  virtual bool is_united(unsigned int member1, unsigned int member2) = 0;
  virtual void print_unions() = 0;

};

class SlowUnion : public Union
{
public:
  SlowUnion(unsigned int num_of_members = 0) : Union(num_of_members) {}

  virtual void unite(unsigned int id1, unsigned int id2) {
    assert((id1 < _size) && (id2 < _size));

    if (members[id1] == members[id2]) return;

    auto changing = members[id2];
    for (unsigned int i = 0; i < _size; i++)
      if (members[i] == changing) members[i] = members[id1];
  }

  virtual bool is_united(unsigned int id1, unsigned int id2){
    return (members[id1] == members[id2]);
  } 

  virtual void print_unions(){
    for (unsigned int i = 0; i < _size; i++) {
      bool found = false;
      for (unsigned int k = 0; k < _size; k++) if (members[k] == i) {
        found ? std::cout << "," : std::cout << "{";
        std::cout << k;
        found = true;
      }
      if (found) std::cout << "} -- ";
    }
    std::cout << std::endl;
  }
};

class FastUnion : public Union
{
private:
  unsigned int find_root(unsigned int member) const {
    while (member != members[member]) member = members[member];
    return member;
  }

public:
  FastUnion(unsigned int num_of_members = 0) : Union(num_of_members) {}

  // Adds root of member1 to root of member 2 
  virtual void unite(unsigned int member1, unsigned int member2) {
    assert((member1 < _size) && (member2 < _size));

    unsigned int root1 = find_root(member1);
    unsigned int root2 = find_root(member2);
    if (root1 == root2) return;
    members[root1] = root2;
  }

  virtual bool is_united(unsigned int member1, unsigned int member2) {
    return (find_root(member1) == find_root(member2));
  }

  virtual void print_unions(){
    unsigned int temp[_size];
    for(unsigned int i = 0; i < _size; i++) temp[i] = 1;

    //Find all roots
    for(unsigned int i = 0; i < _size; i++) {
      if (temp[i] == 0) continue;
      unsigned int root = find_root(i);
      temp[root] = 0; //Mark as visited
      std::cout << "{" << root;
      for (unsigned int k = 0; k < _size; k++) {
        if (find_root(k) == root && k != root) {
          std::cout << "," << k;
          temp[k] = 0;
        }
      }
      std::cout << "} --  ";
    }
    std::cout << std::endl;
  }
};

class FastWeightedUnion : public FastUnion
{
private:
  unsigned int *_treeLength;

  unsigned int find_root(unsigned int member) const {
    while (member != members[member]) {
      // Shrink the tree - for every other member reduce succession by 1
      members[member] = members[members[member]];
      member = members[member];
    }
    return member;
  }

public:

  FastWeightedUnion(unsigned int num_of_members = 0) : FastUnion(num_of_members) {
    // Allocate space for keeping track of the trees and naturally set it to 1
    _treeLength = new unsigned int[num_of_members];
    for (unsigned int i = 0; i < num_of_members; i++) _treeLength[i] = 1;
  }

  ~FastWeightedUnion() {
    delete[] _treeLength;
  }

  virtual void unite(unsigned int member1, unsigned int member2) {
    assert((member1 < _size) && (member2 < _size));

    unsigned int root1 = find_root(member1);
    unsigned int root2 = find_root(member2);
    if (root1 == root2) return;
    if (_treeLength[root1] <= _treeLength[root2]) {
      members[root1] = root2;
      _treeLength[root2] += _treeLength[root1];
    } else {
      members[root2] = root1;
      _treeLength[root1] += _treeLength[root2];
    }
  }

  void print_tree_length() {
    for (unsigned int i = 0; i < _size; i++) {
      std::cout << i << ":" << _treeLength[find_root(i)] << " ";
    }
    std::cout << std::endl;
  }

};
