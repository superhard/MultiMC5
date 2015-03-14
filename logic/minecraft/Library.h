#pragma once
#include <QString>
#include <QPair>
#include <QList>
#include <QStringList>
#include <QMap>
#include <QDir>
#include <QUrl>
#include <QSet>
#include <memory>

#include "minecraft/OneSixRule.h"
#include "minecraft/OpSys.h"
#include "GradleSpecifier.h"
#include "net/URLConstants.h"
#include "net/DownloadableResource.h"

using LibraryPtr = std::shared_ptr<class Library>;

class Library : public BaseDownload
{
public:
	virtual ~Library()
	{
	}

public: /* methods */
	/// Returns the raw name field
	const GradleSpecifier &rawName() const
	{
		return m_name;
	}

	void setRawName(const GradleSpecifier &spec)
	{
		m_name = spec;
	}

	void setClassifier(const QString &spec)
	{
		m_name.setClassifier(spec);
	}

	/// returns the full group and artifact prefix
	QString artifactPrefix() const
	{
		return m_name.artifactPrefix();
	}

	/// get the artifact ID
	QString artifactId() const
	{
		return m_name.artifactId();
	}

	/// get the artifact version
	QString version() const
	{
		return m_name.version();
	}

	/// Set the url base for downloads
	void setBaseUrl(const QUrl &base_url)
	{
		m_base_url = base_url;
	}

	/// List of files this library describes. Required because of platform-specificness of
	/// native libs
	QStringList files() const;

	/// List Shortcut for checking if all the above files exist
	bool filesExist(const QDir &base) const;

	void setAbsoluteUrl(const QUrl &absolute_url)
	{
		m_absolute_url = absolute_url;
	}

	QUrl absoluteUrl() const
	{
		return m_absolute_url;
	}

	virtual void applyTo(const LibraryPtr &other);

	/// Returns true if the library is native
	bool isNative() const
	{
		return m_native_classifiers.size() != 0;
	}

	/// Get the relative path where the library should be saved
	QString storagePath() const;

	void setHint(const QString &hint)
	{
		m_hint = hint;
	}

	QString hint() const
	{
		return m_hint;
	}

	/// Set the load rules
	void setRules(QList<std::shared_ptr<Rule>> rules)
	{
		m_rules = rules;
	}

	/// Returns true if the library should be loaded (or extracted, in case of natives)
	bool isActive() const;

public: /* data */
	// TODO: make all of these protected, clean up semantics of implicit vs. explicit values.
	/// the basic gradle dependency specifier.
	GradleSpecifier m_name;

	/// URL where the file can be downloaded
	QUrl m_base_url;

	/// absolute URL. takes precedence the normal download URL, if defined
	QUrl m_absolute_url;

	/// used for '+' libraries, determines how to add them
	enum InsertType
	{
		Apply,
		Append,
		Prepend,
		Replace
	} insertType = Prepend;
	QString insertData;

	/// determines how can libraries be applied. conflicting dependencies cause errors.
	enum DependType
	{
		Soft, //! needs equal or newer version
		Hard  //! needs equal version (different versions mean version conflict)
	} dependType = Soft;

	/// type hint - modifies how the library is treated
	QString m_hint;

	/// true if the library had an extract/excludes section (even empty)
	bool applyExcludes = false;

	/// a list of files that shouldn't be extracted from the library
	QStringList extract_excludes;

	/// native suffixes per OS
	QMap<OpSys, QString> m_native_classifiers;

	/// true if the library had a rules section (even empty)
	bool applyRules = false;

	/// rules associated with the library
	QList<std::shared_ptr<Rule>> m_rules;

	// BaseDownload interface
public:
	QUrl url() const override;
	void load(const QJsonObject &data) override;
	QList<NetActionPtr> createNetActions() const override;
};
