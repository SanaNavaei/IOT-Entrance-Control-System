#include "authenticator.h"

Authenticator::Authenticator(QString permittedTagsPath, QObject *parent)
    : QObject{parent}
{
    m_permittedTagsPath = permittedTagsPath;
    loadPermittedTags();
}

Authenticator::~Authenticator()
{
    savePermittedTags();
}

bool Authenticator::isPermitted(const QString &tag)
{
    return m_permittedTags.contains(tag);
}

void Authenticator::loadPermittedTags()
{
    QFile file(m_permittedTagsPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Could not open permitted tags file for reading";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        m_permittedTags.append(in.readLine());
    }

    file.close();
}

void Authenticator::savePermittedTags()
{
    QFile file(m_permittedTagsPath);
    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "Could not open permitted tags file for writing";
        return;
    }

    QTextStream out(&file);
    for (const QString &tag : qAsConst(m_permittedTags))
    {
        out << tag << '\n';
    }

    file.close();
}
