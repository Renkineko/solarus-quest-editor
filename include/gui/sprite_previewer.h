/*
 * Copyright (C) 2014-2015 Christopho, Solarus - http://www.solarus-games.org
 *
 * Solarus Quest Editor is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Solarus Quest Editor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef SOLARUSEDITOR_SPRITE_PREVIEWER_H
#define SOLARUSEDITOR_SPRITE_PREVIEWER_H

#include "ui_sprite_previewer.h"
#include <QWidget>
#include <QPointer>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include "sprite_model.h"

/**
 * @brief A widget to preview animation directions of sprites.
 */
class SpritePreviewer : public QWidget
{
  Q_OBJECT

public:

  SpritePreviewer(QWidget* parent = 0);

  void set_model(SpriteModel* model);

private slots:

  void update_selection();
  void update_frame_delay();

  void update_buttons();
  void update_frames();
  void update_frame();

  void timeout();

  void start();
  void stop();
  void first();
  void previous();
  void last();
  void next();

private:

  Ui::SpritePreviewer ui;       /**< The widgets. */
  QPointer<SpriteModel> model;  /**< The sprite model. */
  SpriteModel::Index index;     /**< The selected index. */
  QTimer timer;                 /**< The timer to animate the view. */
  QList<QPixmap> frames;        /**< Frames of the current direction. */
  int current_frame;
  QGraphicsPixmapItem* item;

};

#endif
