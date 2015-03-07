// Licensed under the Apache-2.0 license. See README.md for details.

#include "MoveFileTask.h"

#include <QDir>

#include "FileSystem.h"

MoveFileTask::MoveFileTask(const QString &from, const QString &to, QObject *parent)
	: Task(parent), m_from(from), m_to(to)
{
}
MoveFileTask::MoveFileTask(QObject *parent)
	: Task(parent)
{
}

void MoveFileTask::executeTask()
{
	if (FS::exists(m_to))
	{
		FS::remove(m_to);
	}
	FS::ensureExists(QFileInfo(m_to).dir());
	FS::move(m_from, m_to);
	emitSucceeded();
}
