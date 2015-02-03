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
#include "gui/gui_tools.h"
#include "gui/map_scene.h"
#include "map_model.h"
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QPainter>

/**
 * @brief Graphic item representing a map entity.
 */
class EntityItem : public QGraphicsPixmapItem {

public:

  EntityItem(MapModel& model, const EntityIndex& index);

  // Enable the use of qgraphicsitem_cast with this item.
  enum {
    Type = UserType + 2
  };

  virtual int type() const override {
    return Type;
  }

  EntityIndex get_index() const;
  void set_index(const EntityIndex& index);

protected:

  virtual void paint(QPainter* painter,
                     const QStyleOptionGraphicsItem* option,
                     QWidget* widget = nullptr) override;

private:

  MapModel& model;            /**< The map this entity belongs to. */
  EntityIndex index;          /**< Index of the entity in the map. */

};

/**
 * @brief Creates a map scene.
 * @param model The map data to represent in the scene.
 * @param parent The parent object or nullptr.
 */
MapScene::MapScene(MapModel& model, QObject* parent) :
  QGraphicsScene(parent),
  model(model) {

  build();

}

/**
 * @brief Returns the map represented in the scene.
 * @return The map.
 */
const MapModel& MapScene::get_model() const {
  return model;
}

/**
 * @brief Returns the quest the map belongs to.
 * @return The quest.
 */
const Quest& MapScene::get_quest() const {
  return model.get_quest();
}

/**
 * @brief Create all entity items in the scene.
 */
void MapScene::build() {


  setSceneRect(QRectF(QPoint(0, 0), model.get_size()));
  for (int i = 0; i < Layer::LAYER_NB; ++i) {
    Layer layer = static_cast<Layer>(i);
    entity_items[i].clear();
    for (int j = 0; j < model.get_num_entities(layer); ++j) {
      create_entity_item(EntityIndex(layer, j));
    }
  }

}

/**
 * @brief Creates a graphic item for the specified entity.
 * @param index Index of a map entity.
 */
void MapScene::create_entity_item(const EntityIndex& index) {

  EntityItem* item = new EntityItem(model, index);
  addItem(item);
  entity_items[index.layer].append(item);
}

/**
 * @brief Creates an entity item.
 * @param model The map.
 * @param index Index of the entity on the map.
 */
EntityItem::EntityItem(MapModel& model, const EntityIndex& index) :
  QGraphicsPixmapItem(model.get_entity_image(index)),
  model(model),
  index(index) {

  QRect frame = model.get_entity_bounding_box(index);
  setPos(frame.topLeft());
  setFlags(ItemIsSelectable | ItemIsFocusable);
}

/**
 * @brief Returns the index of the entity on the map.
 * @return The entity index.
 */
EntityIndex EntityItem::get_index() const {
  return index;
}

/**
 * @brief Sets the index of the entity on the map.
 * @param index The new index.
 */
void EntityItem::set_index(const EntityIndex& index) {
  this->index = index;
}

/**
 * @brief Paints the pattern item.
 *
 * Reimplemented to draw our own selection marker.
 *
 * @param painter The painter.
 * @param option Style option of the item.
 * @param widget The widget being painted or nullptr.
 */
void EntityItem::paint(QPainter* painter,
                        const QStyleOptionGraphicsItem* option,
                        QWidget* widget) {

  if (!model.entity_exists(index)) {
    // Bug in the editor.
    qCritical() << MapScene::tr("No such entity index on layer %1: %2").arg(index.layer, index.index);
    return;
  }

  QRect box = model.get_entity_bounding_box(index);
  QPoint top_left = box.topLeft();
  box.translate(-top_left);

  // Start with an opaque background, to erase anything below
  // if the pattern has transparency.
  painter->fillRect(box, widget->palette().window());

  const bool selected = option->state & QStyle::State_Selected;

  // First, paint the item like if there was no selection, to avoid
  // Qt's built-in selection marker.
  QStyleOptionGraphicsItem option_deselected = *option;
  option_deselected.state &= ~QStyle::State_Selected;
  QGraphicsPixmapItem::paint(painter, &option_deselected, widget);

  // Add our selection marker.
  if (selected) {
    GuiTools::draw_rectangle_outline(*painter, box, Qt::blue);
  }
}