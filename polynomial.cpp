#include "polynomial.h"

ostream &operator<<(ostream &output, const Polynomial &p) {
  Polynomial::Term *temp = p.head->next;
  while (temp != p.head) {
    output << temp->coeff;
    if (temp->power != 0) {
      output << "x^" << temp->power;
    }
    if (temp->next != p.head && temp->next->coeff >= 0) {
      output << " + ";
    } else if (temp->next != p.head && temp->next->coeff < 0) {
      output << " - ";
    }
    temp = temp->next;
  }
  return output;
}

Polynomial::Polynomial() {
  size = 0;
  head = new Term;
  head->prev = head;
  head->next = head;
}

Polynomial::Polynomial(const Polynomial &p) { *this = p; }

Polynomial::~Polynomial() {}

int Polynomial::degree() const { return head->next->power; }

double Polynomial::coefficient(const int power) const {
  Term *curr = head;
  while (curr != head) {
    if (curr->power == power) {
      return curr->coeff;
    } else {
      curr = curr->next;
    }
  }
  return -1; // not found
}

bool Polynomial::changeCoefficient(const double newCoefficient,
                                   const int power) {
  Term *curr = head->next;
  while (curr != head && curr->power != power) {
    curr = curr->next;
  }
  if (curr->power == power) {
    curr->coeff = newCoefficient;
    if (newCoefficient == 0) {
      return remove(curr);
    }
    return true;
  } else {
    return insert(curr, newCoefficient, power);
  }
}

Polynomial Polynomial::operator+(const Polynomial &p) const {
  Term *p1 = head->next;
  Term *p2 = p.head->next;
  Polynomial ans;
  while (p1 != head && p2 != head) {
    if (p1->power > p2->power) {
      ans.changeCoefficient(p1->coeff, p1->power);
      p1 = p1->next;
    } else if (p1->power < p2->power) {
      ans.changeCoefficient(p2->coeff, p2->power);
      p2 = p2->next;
    } else {
      ans.changeCoefficient(p1->coeff + p2->coeff, p1->power);
      p1 = p1->next;
      p2 = p2->next;
    }
  }
  return ans;
}

Polynomial Polynomial::operator-(const Polynomial &p) const {
  Term *p1 = head->next;
  Term *p2 = p.head->next;
  Polynomial ans;
  while (p1 != head && p2 != head) {
    if (p1->power > p2->power) {
      ans.changeCoefficient(p1->coeff, p1->power);
      p1 = p1->next;
    } else if (p1->power < p2->power) {
      ans.changeCoefficient(p2->coeff, p2->power);
      p2 = p2->next;
    } else {
      ans.changeCoefficient(p1->coeff - p2->coeff, p1->power);
      p1 = p1->next;
      p2 = p2->next;
    }
  }
  return ans;
}

bool Polynomial::operator==(const Polynomial &p) const {
  if (degree() == p.degree()) {
    return true;
  } else {
    return false;
  }
  for (int i = degree(); i >= 0; i--) {
    if (coefficient(i) != p.coefficient(i)) {
      return false;
    }
  }
  return true;
}

bool Polynomial::operator!=(const Polynomial &p) const { return !(p == *this); }

Polynomial &Polynomial::operator=(const Polynomial &p) {
  if (p == *this) {
    return *this;
  }
  Term *p1 = head;
  Term *p2 = p.head->next;
  while (p2 != p.head) {
    if (p1->power == p2->power) {
      p1->coeff = p2->coeff;
      if (p2->coeff == 0) {

        remove(p1);
      }
      p1 = p1->next;
    } else if (p1->power < p2->power) {

      insert(p1, p2->coeff, p2->power);
      p1 = p1->next;
    } else {

      p1 = p1->next;
    }
    p2 = p2->next;
  }

  return *this;
}

bool Polynomial::insert(Term *pos, const double newCoefficient,
                        const int power) {
  Term *insert = new Term;
  insert->coeff = newCoefficient;
  insert->power = power;

  insert->prev = pos->prev;
  insert->next = pos;
  pos->prev->next = insert;
  pos->prev = insert;
  size++;
  return true;
}

bool Polynomial::remove(Term *pos) {
  if (pos == nullptr || size == 0) {
    return false;
  }
  pos->prev->next = pos->next;
  pos->next->prev = pos->prev;
  delete pos;
  size--;
  return true;
}
