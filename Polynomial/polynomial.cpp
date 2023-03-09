#include <cmath>
#include "polynomial.h"

int Polynomial::Monomial::convert_to_power(int p1, int p2, int p3) {
	p1 -= min_power;
	p2 -= min_power;
	p3 -= min_power;
	return d * d * p1 + d * p2 + p3;
}

void Polynomial::Monomial::convert_back(int power, int& p1, int& p2, int& p3) {
	p3 = power % d + min_power;
	power /= d;
	p2 = power % d + min_power;
	power /= d;
	p1 = power % d + min_power;
}

void Polynomial::add_last(double coef, int power) {
	if (start == nullptr) {
		start = new Monomial(coef, power);
		last = start;
	}
	else {
		last->next = new Monomial(coef, power);
		last = last->next;
	}
}

void Polynomial::add_monomial(double coef, int power) {
	if (start == nullptr) {
		add_last(coef, power);
		return;
	}
	if (start->power == power) {
		start->coef += coef;
		return;
	}
	if (start->power > power) {
		start = new Monomial(coef, power, start);
		return;
	}
	Monomial* now = start;
	while (now->next != nullptr) {
		if (now->next->power == power) {
			now->next->coef += coef;
			return;
		}
		if (now->next->power > power) {
			now->next = new Monomial(coef, power, now->next);
			return;
		}
		now = now->next;
	}
	add_last(coef, power);
}

void Polynomial::delete_zero_monomials() {
	while (start != nullptr && start->coef == 0.0) {
		auto temp = start->next;
		start->next = nullptr;
		delete start;
		start = temp;
	}
	if (start == nullptr) {
		last = nullptr;
		return;
	}
	Monomial* now = start;
	while (now->next != nullptr) {
		while (now->next != nullptr && now->next->coef == 0.0) {
			auto temp = now->next->next;
			now->next->next = nullptr;
			delete now->next;
			now->next = temp;
		}
		if (now->next != nullptr)
			now = now->next;
	}
	last = now;
}

double Polynomial::calculate(double x, double y, double z) const {
	double ans = 0.0;
	Monomial* now = start;
	while (now != nullptr) {
		int p[3]{};
		Monomial::convert_back(now->power, p[0], p[1], p[2]);
		ans += now->coef * std::exp(p[0] * std::log(x)) *
			std::exp(p[1] * std::log(y)) * std::exp(p[2] * std::log(z));
		now = now->next;
	}
	return ans;
}

Polynomial& Polynomial::operator=(const Polynomial& p)
{
	if (this == &p)
		return *this;
	clear();
	if (p.start == nullptr) {
		last = start = nullptr;
		return *this;
	}
	start = new Monomial(*p.start);
	last = start;
	while (last->next != nullptr)
		last = last->next;
	return *this;
}

Polynomial Polynomial::operator+() const {
	return *this;
}

Polynomial Polynomial::operator-() const {
	Polynomial ret = Polynomial(*this);
	auto now = ret.start;
	while (now != nullptr) {
		now->coef = -now->coef;
		now = now->next;
	}
	return ret;
}

Polynomial Polynomial::operator+(const Polynomial& p) const {
	Polynomial ret;
	Monomial* now1 = this->start;
	Monomial* now2 = p.start;
	while(now1 != nullptr && now2 != nullptr) {
		if (now1->power == now2->power) {
			ret.add_last(now1->coef + now2->coef, now1->power);
			now1 = now1->next;
			now2 = now2->next;
		}
		else if (now1->power < now2->power) {
			ret.add_last(now1->coef, now1->power);
			now1 = now1->next;
		}
		else {
			ret.add_last(now2->coef, now2->power);
			now2 = now2->next;
		}
	}
	while (now1 != nullptr) {
		ret.add_last(now1->coef, now1->power);
		now1 = now1->next;
	}
	while (now2 != nullptr) {
		ret.add_last(now2->coef, now2->power);
		now2 = now2->next;
	}
	ret.delete_zero_monomials();
	return ret;
}

Polynomial Polynomial::operator-(const Polynomial& p) const {
	return *this + -p;
}

Polynomial Polynomial::operator*(const Polynomial& p) const {
	Polynomial ret;
	Monomial* now1 = this->start;
	while (now1 != nullptr)	{
		Monomial* now2 = p.start;
		while (now2 != nullptr)	{
			int r1[3]{}, r2[3]{};
			Monomial::convert_back(now1->power, r1[0], r1[1], r1[2]);
			Monomial::convert_back(now2->power, r2[0], r2[1], r2[2]);
			ret.add_monomial(now1->coef * now2->coef, r1[0] + r2[0], r1[1] + r2[1], r1[2] + r2[2]);
			now2 = now2->next;
		}
		now1 = now1->next;
	}
	ret.delete_zero_monomials();
	return ret;
}

std::istream& operator>>(std::istream& in, Polynomial& p) {
	p.clear();
	char c;
	in.get(c);
	while (c != ';' && c != '\n' && !in.eof()) {
		std::string s_coef;
		if (c == '-' || c == '+') {
			s_coef += c;
			in.get(c);
		}
		while (std::isdigit(c)) {
			s_coef += c;
			in.get(c);
		}
		if (c == '.') {
			s_coef += c;
			in.get(c);
		}
		while (std::isdigit(c)) {
			s_coef += c;
			in.get(c);
		}
		if (s_coef == "+" || s_coef == "-" || s_coef.empty())
			s_coef += '1';
		double coef = std::stod(s_coef);
		int r[3]{};
		while (c == 'x' || c == 'y' || c == 'z') {
			int id = c - 'x';
			in.get(c);
			if (c != '^')
				throw "unexpected symbol";
			std::string s_r;
			in.get(c);
			if (c == '-' || c == '+') {
				s_r += c;
				in.get(c);
			}
			while (std::isdigit(c)) {
				s_r += c;
				in.get(c);
			}
			r[id] += std::stoi(s_r);
		}

		p.add_monomial(coef, r[0], r[1], r[2]);

		if (c != '+' && c != '-' && c != '\n' && c != ';') {
			throw "unexpected symbol";
		}
	}
	p.delete_zero_monomials();
	return in;
}

std::ostream& operator<<(std::ostream& out, const Polynomial& p) {
	auto now = p.start;
	int cnt = 0;
	while (now != nullptr) {
		int r[3]{};
		now->convert_back(now->power, r[0], r[1], r[2]);
		if (cnt > 0 && now->coef >= 0)
			out << '+';
		out << now->coef;
		for (int i = 0; i < 3; i++)
			if (r[i])
				out << (char)('x' + i) << '^' << r[i];
		now = now->next;
		cnt++;
	}
	return out;
}
