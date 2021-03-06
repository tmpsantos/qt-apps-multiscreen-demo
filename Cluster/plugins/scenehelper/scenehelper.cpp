/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt multiscreen demo application.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "scenehelper.h"

QObject *SceneHelper::findEntity(Qt3DRender::QSceneLoader *loader, const QString &name)
{
    // The QSceneLoader instance is a component of an entity. The loaded scene
    // tree is added under this entity.
    QVector<Qt3DCore::QEntity *> entities = loader->entities();

    if (entities.isEmpty())
        return 0;

    // Technically there could be multiple entities referencing the scene loader
    // but sharing is discouraged, and in our case there will be one anyhow.
    Qt3DCore::QEntity *root = entities[0];

    // The scene structure and names always depend on the asset.
    return root->findChild<Qt3DCore::QEntity *>(name);
}

void SceneHelper::removeFromScene(Qt3DRender::QSceneLoader *loader, const QStringList &names)
{
    //qDebug() << __FUNCTION__ << names;
    QVector<Qt3DCore::QEntity *> entities = loader->entities();

    if (entities.isEmpty())
        return;

    Qt3DCore::QEntity *root = entities[0];

    foreach (QString name, names) {
        QObject *entity = root->findChild<Qt3DCore::QEntity *>(name);
        entity->setParent(Q_NULLPTR);
    }
}

void SceneHelper::addMaterials(Qt3DRender::QSceneLoader *loader,
                               Qt3DRender::QMaterial *material, QStringList names)
{
    //qDebug() << __FUNCTION__ << material << names;
    QVector<Qt3DCore::QEntity *> entities = loader->entities();

    if (entities.isEmpty())
        return;

    Qt3DCore::QEntity *root = entities[0];

    addComponents(names, root, material);
}

void SceneHelper::addMaterial(Qt3DRender::QSceneLoader *loader,
                              Qt3DRender::QMaterial *material, QString name)
{
    //qDebug() << __FUNCTION__ << material << name;
    QVector<Qt3DCore::QEntity *> entities = loader->entities();

    //qDebug() << entities;

    if (entities.isEmpty())
        return;

    Qt3DCore::QEntity *root = entities[0];

    addComponent(name, root, material);
}

void SceneHelper::replaceMaterial(Qt3DRender::QSceneLoader *loader, const QString &name,
                                  Qt3DRender::QMaterial *material)
{
    //qDebug() << __FUNCTION__ << "Element " << name << " to " << material;
    QVector<Qt3DCore::QEntity *> entities = loader->entities();

    if (entities.isEmpty())
        return;

    Qt3DCore::QEntity *root = entities[0];
    Qt3DCore::QEntity *entity = root->findChild<Qt3DCore::QEntity *>(name);
    if (entity) {
        QVector<Qt3DCore::QComponent *> components = entity->components();
        //qDebug() << components;
        foreach (Qt3DCore::QComponent *comp, components) {
            if (qobject_cast<Qt3DRender::QMaterial *>(comp)) {
                //qDebug() << "removing material";
                entity->removeComponent(comp);
                break;
            }
        }
        //qDebug() << "adding" << material;
        entity->addComponent(material);
    } else {
        qWarning() << __FUNCTION__ << "MISSING ELEMENT " << name;
    }
}

void SceneHelper::addComponents(QStringList &names, Qt3DCore::QEntity *root,
                                Qt3DRender::QMaterial *material)
{
    //qDebug() << __FUNCTION__ << material;
    foreach (QString name, names) {
        Qt3DCore::QEntity *entity = root->findChild<Qt3DCore::QEntity *>(name);
        //qDebug() << "entity = " << entity << " name " << name;
        if (entity) {
            QVector<Qt3DCore::QComponent *> components = entity->components();
            foreach (Qt3DCore::QComponent *comp, components) {
                //qDebug() << "   comp = " << comp;
                if (qobject_cast<Qt3DRender::QMaterial *>(comp)) {
                    //qDebug() << "   removing " << comp;
                    entity->removeComponent(comp);
                    break;
                }
            }
            //qDebug() << "   adding " << material;
            entity->addComponent(material);
        } else {
            qWarning() << __FUNCTION__ << "MISSING ELEMENT " << name;
        }
    }
}

/*void SceneHelper::searchCamera(Qt3DRender::QSceneLoader *loader, const QString &name)
{
    QVector<Qt3DCore::QEntity *> entities = loader->entities();

    if (entities.isEmpty())
        return;

    Qt3DCore::QEntity *root = entities[0];
    Qt3DCore::QEntity *entity = root->findChild<Qt3DCore::QEntity *>(name);
    //qDebug() << "Found entity " << entity;
    QVector<Qt3DCore::QComponent *> components = entity->components();
    foreach (Qt3DCore::QComponent *comp, components) {
        //qDebug() << "   comp " << comp;
        if (qobject_cast<Qt3DRender::QCameraLens *>(comp)) {
            Qt3DRender::QCameraLens *lens = qobject_cast<Qt3DRender::QCameraLens *>(comp);
            //qDebug() << "   lens.left = " << lens->left() << lens->property("lookAt");
        }
    }
}*/

void SceneHelper::addComponent(QString &name, Qt3DCore::QEntity *root,
                               Qt3DRender::QMaterial *material)
{
    //qDebug() << __FUNCTION__ << material;
    Qt3DCore::QEntity *entity = root->findChild<Qt3DCore::QEntity *>(name);
    //qDebug() << "entity = " << entity << " name " << name;
    if (entity) {
        QVector<Qt3DCore::QComponent *> components = entity->components();
        foreach (Qt3DCore::QComponent *comp, components) {
            //qDebug() << "   comp = " << comp;
            if (qobject_cast<Qt3DRender::QMaterial *>(comp)) {
                //qDebug() << "   removing " << comp;
                entity->removeComponent(comp);
                break;
            }
        }
        //qDebug() << "   adding " << material;
        entity->addComponent(material);
    } else {
        qWarning() << __FUNCTION__ << "MISSING ELEMENT " << name;
    }
}

void SceneHelper::addListEntry(const QVariant &list, QObject *entry)
{
    QQmlListReference ref = list.value<QQmlListReference>();
    ref.append(entry);
}
