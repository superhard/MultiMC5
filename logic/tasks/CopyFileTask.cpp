// Licensed under the Apache-2.0 license. See README.md for details.

#include "CopyFileTask.h"

#include <QDir>

#include "FileSystem.h"

CopyFileTask::CopyFileTask(const QString &from, const QString &to, QObject *parent)
	: Task(parent), m_from(from), m_to(to)
{
}
CopyFileTask::CopyFileTask(QObject *parent)
	: Task(parent)
{
}

static QStringList gatherFiles(const QDir &dir, const QDir &root)
{
	QStringList out;
	for (const QFileInfo &entry : dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs))
	{
		out += root.relativeFilePath(entry.absoluteFilePath());
		if (entry.isDir())
		{
			out += gatherFiles(entry.absoluteFilePath(), root);
		}
	}
	return out;
}

void CopyFileTask::executeTask()
{
	if (!FS::exists(m_to))
	{
		if (QFileInfo(m_from).isFile())
		{
			FS::ensureExists(QFileInfo(m_to).dir());
			FS::copy(m_from, m_to);
		}
		else
		{
			const QStringList files = gatherFiles(m_from, m_from);
			const int total = files.size();
			int current = 0;
			for (const auto file : files)
			{
				setProgress(current++ / total);

				const QFileInfo info(m_from, file);
				if (info.isDir())
				{
					FS::ensureExists(info.absoluteFilePath());
				}
				else
				{
					FS::copy(info.absoluteFilePath(), QDir(m_to).absoluteFilePath(file));
				}
			}
		}
	}
	emitSucceeded();
}
