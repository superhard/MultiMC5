/* Copyright 2013-2015 MultiMC Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <QString>
#include <QList>
#include <QJsonObject>
#include <memory>
#include "OpSys.h"

class Library;
class Rule;

enum RuleAction
{
	Allow,
	Disallow,
	Defer
};

class Rule
{
public:
	RuleAction m_result;
	Rule(RuleAction result) : m_result(result)
	{
	}
	virtual ~Rule() {}
	virtual bool applies() = 0;
	RuleAction apply()
	{
		if (applies())
			return m_result;
		else
			return Defer;
	}
};

class OsRule : public Rule
{
public:
	static std::shared_ptr<OsRule> create(RuleAction result, OpSys system, QString version_regexp)
	{
		return std::shared_ptr<OsRule>(new OsRule(result, system, version_regexp));
	}
	OsRule(RuleAction result, OpSys system, QString version_regexp)
		: Rule(result), m_system(system), m_version_regexp(version_regexp)
	{
	}
	virtual ~OsRule(){}
	virtual bool applies()
	{
		return (m_system == currentSystem);
	}

	// the OS
	OpSys m_system;
	// the OS version regexp
	QString m_version_regexp;
};

class ImplicitRule : public Rule
{
public:
	static std::shared_ptr<ImplicitRule> create(RuleAction result)
	{
		return std::shared_ptr<ImplicitRule>(new ImplicitRule(result));
	}
	ImplicitRule(RuleAction result) : Rule(result)
	{
	}
	virtual ~ImplicitRule() {}
	virtual bool applies()
	{
		return true;
	}
};
