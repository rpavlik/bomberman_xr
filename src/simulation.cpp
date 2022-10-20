#include "tom_engine.h"

constexpr float global_scale = 0.25f;

struct HandControllers {
  GraphicsMeshInstanceArray mesh_instance_array;
  uint32_t material_id;

  void init() {
    this->material_id = create_graphics_material(Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{0.0f, 0.0f, 0.0f}, 0.0f, 0.53f, "assets/textures/quest_2_controllers.png");
    create_graphics_mesh_instance_array_from_glb("assets/models/quest_2_controllers.glb", this->mesh_instance_array);
  }

  void update() {
    Transform left_hand_transform  = input_state.left_hand_transform;
    Transform right_hand_transform = input_state.right_hand_transform;

    Vector3f y_axis = { 0.0f,1.0f,0.0f };
    Vector3f x_axis = { 1.0f,0.0f,0.0f };

    left_hand_transform.orientation  = identity_orientation;
    right_hand_transform.orientation = identity_orientation;
    rotate_transform_global(right_hand_transform, 180.0f, y_axis);
    rotate_transform_global(right_hand_transform, -60.0f, x_axis);
    rotate_transform_global(right_hand_transform, input_state.right_hand_transform.orientation);

    rotate_transform_global(left_hand_transform, 180.0f, y_axis);
    rotate_transform_global(left_hand_transform, -60.0f, x_axis);
    rotate_transform_global(left_hand_transform, input_state.left_hand_transform.orientation);

    Vector3f translation_offset_left  = {-0.008f,0.0225f,0.0225f};
    Vector3f translation_offset_right = {0.008f,0.0225f,0.0225f};
    translate_transform_local(left_hand_transform, translation_offset_left);
    translate_transform_local(right_hand_transform, translation_offset_right);

    const float scale = 0.01f;
    left_hand_transform.scale  = {scale, scale, scale};
    right_hand_transform.scale = {scale, scale, scale};

    update_graphics_mesh_instance_array(this->mesh_instance_array, left_hand_transform, material_id, uint32_t(-1), 0);
    update_graphics_mesh_instance_array(this->mesh_instance_array, right_hand_transform, material_id, uint32_t(-1), 1);
  };
};

struct BomberMan {
  Vector3f position;
  Quaternionf orientation;
  uint32_t material_id;
  uint32_t sound_id;
  GraphicsSkin skin;
  AnimationArray animations;

  void init() {
    this->orientation = identity_orientation;
    this->position = {0.0f,0.0f,0.0f};

    this->material_id = create_graphics_material(Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{0.0f, 0.0f, 0.0f}, 0.0f, 1.0f, "assets/textures/bomberman.png");
    create_graphics_skin_from_glb("assets/models/bomberman.glb", this->skin);

    this->sound_id = create_audio_source("assets/sounds/white_bomberman_hurt.wav", 4.0f, 16.0f, 0.0f, 1.0f);
    this->skin.update_all(material_id);
    this->animations = load_animations_from_glb_file("assets/models/bomberman.glb");
  }

  void update() {
    Transform transform = {this->position,this->orientation,{0.00035f * global_scale, 0.00035f * global_scale, 0.00035f * global_scale}};

    Vector3f x_axis = { 1.0f, 0.0f, 0.0f };
    rotate_transform_global(transform, 90.0f, x_axis);

    update_audio_source(sound_id, position);

    this->skin.update(0, transform, material_id);
  }
};

struct Bomb {
  Vector3f position;
  Quaternionf orientation;
  uint32_t material_id;
  uint32_t sound_id;
  GraphicsMeshInstanceArray mesh_instance_array;

  void update() {
    Transform transform = {this->position,this->orientation,{0.1f * global_scale, 0.1f * global_scale, 0.1f * global_scale}};
    for (uint32_t i=0; i < this->mesh_instance_array.size; ++i) update_graphics_mesh_instance_array(this->mesh_instance_array, transform, material_id, uint32_t(-1), i);
  }
};

struct Fire {
  Vector3f position;
  Quaternionf orientation;
  uint32_t material_id;
  uint32_t sound_id;
  GraphicsSkin skin;
  AnimationArray animations;

  /*
  void init() {
    this->orientation = identity_orientation;
    this->position = {0.0f,1.0f,0.0f};

    this->material_id = create_graphics_material(Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{0.0f, 0.0f, 0.0f}, 0.0f, 0.8f, "assets/textures/fire.png");
    create_graphics_skin_from_glb("assets/models/fire.glb", this->skin);

    this->skin.update_all(material_id);
    this->animations = load_animations_from_glb_file("assets/models/fire.glb");
  }
  */

  void update() {
    Transform transform = {this->position,this->orientation,{0.1f * global_scale, 0.1f * global_scale, 0.1f * global_scale}};
    Vector3f x_axis = { 1.0f, 0.0f, 0.0f };
    rotate_transform_global(transform, 90.0f, x_axis);
    this->skin.update(0, transform, material_id);
  }
};

struct BrickBlock {
  Vector3f position;
  Quaternionf orientation;
  uint32_t material_id_0;
  uint32_t material_id_1;
  GraphicsMeshInstanceArray mesh_instance_array;

  void update() {
    Transform transform = {this->position,this->orientation,{0.0001125f * global_scale, 0.0001125f * global_scale, 0.045f * global_scale}};
    update_graphics_mesh_instance_array(this->mesh_instance_array, transform, material_id_0, uint32_t(-1), 0);
    update_graphics_mesh_instance_array(this->mesh_instance_array, transform, material_id_1, uint32_t(-1), 1);
  }
};

struct StoneBlock {
  Vector3f position;
  Quaternionf orientation;
  uint32_t material_id;
  GraphicsMeshInstanceArray mesh_instance_array;

  void update() {
    Transform transform = {this->position,this->orientation,{0.05f * global_scale, 0.05f * global_scale, 0.05f * global_scale}};
    update_graphics_mesh_instance_array(this->mesh_instance_array, transform, material_id, uint32_t(-1), 0);
    for (uint32_t i=0; i < this->mesh_instance_array.size; ++i) update_graphics_mesh_instance_array(this->mesh_instance_array, transform, material_id, uint32_t(-1), i);
  }
};

struct FloorWallBlock {
  Vector3f position;
  Quaternionf orientation;
  uint32_t material_id;
  GraphicsMeshInstanceArray mesh_instance_array;

  void update() {
    Transform transform = {this->position,this->orientation,{0.001f * global_scale, 0.001f * global_scale, 0.001f * global_scale}};
    update_graphics_mesh_instance_array(this->mesh_instance_array, transform, material_id, uint32_t(-1), 0);
    for (uint32_t i=0; i < this->mesh_instance_array.size; ++i) update_graphics_mesh_instance_array(this->mesh_instance_array, transform, material_id, uint32_t(-1), i);
  }
};

struct Board {
  Vector3f first_block_position = {0.0f, 1.0f, 1.0f}; // position of top left floor block
  Vector3f first_floor_position; // position of top left floor block
  const float block_offset_x = 0.105f * global_scale;
  const float block_offset_y = 0.107f * global_scale;
  const float block_offset_z = 0.1045f * global_scale;
  static constexpr size_t floor_wall_block_count = 247; // 13 height and 15 width
  FloorWallBlock floor_wall_blocks[floor_wall_block_count];
  StoneBlock all_stones[30];
  BrickBlock all_bricks[143];
  Bomb all_bombs[143];
  //Fire all_fire[143];

  uint32_t wall_material_id;
  uint32_t floor_1_material_id;
  uint32_t floor_2_material_id;
  uint32_t stone_material_id;
  uint32_t brick_material_id_0;
  uint32_t brick_material_id_1;
  uint32_t bomb_material_id;
  uint32_t fire_material_id;

  // TODO: don't need to set positions on init
  void show_brick(const size_t tile_index) {
    const size_t row_index    = tile_index / 13;
    const size_t column_index = tile_index % 13;
    all_bricks[tile_index].position = { first_floor_position.x + ((float)column_index * block_offset_x), first_floor_position.y + block_offset_y, first_floor_position.z + ((float)row_index * block_offset_z) };
  }

  void hide_brick(const size_t tile_index) {
    all_bricks[tile_index].position = { 1000000.0f, 1000000.0f, 1000000.0f };
  }

  void show_bomb(const size_t tile_index) {
    const size_t row_index    = tile_index / 13;
    const size_t column_index = tile_index % 13;
    all_bombs[tile_index].position = { first_floor_position.x + ((float)column_index * block_offset_x), first_floor_position.y + (block_offset_y / 2.0f), first_floor_position.z + ((float)row_index * block_offset_z) };
  }

  void hide_bomb(const size_t tile_index) {
    all_bombs[tile_index].position = { 1000000.0f, 1000000.0f, 1000000.0f };
  }

  void init() {
    // TODO: review material properties
    this->wall_material_id    = create_graphics_material(Vector4f{0.27843f, 0.27451f, 0.2549f, 1.0f}, 0.0f, 0.8f);
    this->floor_1_material_id = create_graphics_material(Vector4f{0.22353f, 0.43922f, 0.1451f, 1.0f}, 0.0f, 0.8f);
    this->floor_2_material_id = create_graphics_material(Vector4f{0.27843f, 0.52941f, 0.18039f, 1.0f}, 0.0f, 0.8f);
    this->stone_material_id   = create_graphics_material(Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{0.0f, 0.0f, 0.0f}, 0.0f, 0.8f, "assets/textures/block_rock.jpeg");
    this->brick_material_id_0 = create_graphics_material(Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{0.0f, 0.0f, 0.0f}, 0.0f, 0.8f, "assets/textures/brick_texture.png");
    this->brick_material_id_1 = create_graphics_material(Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, 0.0f, 0.8f);
    this->bomb_material_id    = create_graphics_material(Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{0.0f, 0.0f, 0.0f}, 0.0f, 0.8f, "assets/textures/bomb.png");
    this->fire_material_id    = create_graphics_material(Vector4f{1.0f, 1.0f, 1.0f, 1.0f}, Vector3f{0.0f, 0.0f, 0.0f}, 0.0f, 0.8f, "assets/textures/fire.png");

    size_t floor_wall_blocks_index = 0;

    /* create walls */
    const Vector3f top_right_block_position = { first_block_position.x + (14.0f * block_offset_x), first_block_position.y, first_block_position.z };
    const Vector3f bottom_left_block_position = { first_block_position.x, first_block_position.y, first_block_position.z + (12.0f * block_offset_z) };
    for (size_t i=0; i < 15; ++i) {
      floor_wall_blocks[floor_wall_blocks_index].orientation = identity_orientation;
      floor_wall_blocks[floor_wall_blocks_index].position    = { first_block_position.x + ((float)i * block_offset_x), first_block_position.y, first_block_position.z };
      floor_wall_blocks[floor_wall_blocks_index].material_id = wall_material_id;
      create_graphics_mesh_instance_array_from_glb("assets/models/block.glb", floor_wall_blocks[floor_wall_blocks_index].mesh_instance_array);
      ++floor_wall_blocks_index;

      floor_wall_blocks[floor_wall_blocks_index].orientation = identity_orientation;
      floor_wall_blocks[floor_wall_blocks_index].position    = { first_block_position.x + ((float)i * block_offset_x), first_block_position.y + block_offset_y, first_block_position.z };
      floor_wall_blocks[floor_wall_blocks_index].material_id = wall_material_id;
      create_graphics_mesh_instance_array_from_glb("assets/models/block.glb", floor_wall_blocks[floor_wall_blocks_index].mesh_instance_array);
      ++floor_wall_blocks_index;
    }
    for (size_t i=1; i < 13; ++i) {
      floor_wall_blocks[floor_wall_blocks_index].orientation = identity_orientation;
      floor_wall_blocks[floor_wall_blocks_index].position    = { first_block_position.x, first_block_position.y, first_block_position.z + (i * block_offset_z) };
      floor_wall_blocks[floor_wall_blocks_index].material_id = wall_material_id;
      create_graphics_mesh_instance_array_from_glb("assets/models/block.glb", floor_wall_blocks[floor_wall_blocks_index].mesh_instance_array);
      ++floor_wall_blocks_index;

      floor_wall_blocks[floor_wall_blocks_index].orientation = identity_orientation;
      floor_wall_blocks[floor_wall_blocks_index].position    = { first_block_position.x, first_block_position.y + block_offset_y, first_block_position.z + ((float)i * block_offset_z) };
      floor_wall_blocks[floor_wall_blocks_index].material_id = wall_material_id;
      create_graphics_mesh_instance_array_from_glb("assets/models/block.glb", floor_wall_blocks[floor_wall_blocks_index].mesh_instance_array);
      ++floor_wall_blocks_index;
    }
    for (size_t i=1; i < 13; ++i) {
      floor_wall_blocks[floor_wall_blocks_index].orientation = identity_orientation;
      floor_wall_blocks[floor_wall_blocks_index].position    = { top_right_block_position.x, top_right_block_position.y, top_right_block_position.z + ((float)i * block_offset_z) };
      floor_wall_blocks[floor_wall_blocks_index].material_id = wall_material_id;
      create_graphics_mesh_instance_array_from_glb("assets/models/block.glb", floor_wall_blocks[floor_wall_blocks_index].mesh_instance_array);
      ++floor_wall_blocks_index;

      floor_wall_blocks[floor_wall_blocks_index].orientation = identity_orientation;
      floor_wall_blocks[floor_wall_blocks_index].position    = { top_right_block_position.x, top_right_block_position.y + block_offset_y, top_right_block_position.z + ((float)i * block_offset_z) };
      floor_wall_blocks[floor_wall_blocks_index].material_id = wall_material_id;
      create_graphics_mesh_instance_array_from_glb("assets/models/block.glb", floor_wall_blocks[floor_wall_blocks_index].mesh_instance_array);
      ++floor_wall_blocks_index;
    }
    for (size_t i=1; i < 14; ++i) {
      floor_wall_blocks[floor_wall_blocks_index].orientation = identity_orientation;
      floor_wall_blocks[floor_wall_blocks_index].position    = { bottom_left_block_position.x + ((float)i * block_offset_x), bottom_left_block_position.y, bottom_left_block_position.z };
      floor_wall_blocks[floor_wall_blocks_index].material_id = wall_material_id;
      create_graphics_mesh_instance_array_from_glb("assets/models/block.glb", floor_wall_blocks[floor_wall_blocks_index].mesh_instance_array);
      ++floor_wall_blocks_index;

      floor_wall_blocks[floor_wall_blocks_index].orientation = identity_orientation;
      floor_wall_blocks[floor_wall_blocks_index].position    = { bottom_left_block_position.x + ((float)i * block_offset_x), bottom_left_block_position.y + block_offset_y, bottom_left_block_position.z };
      floor_wall_blocks[floor_wall_blocks_index].material_id = wall_material_id;
      create_graphics_mesh_instance_array_from_glb("assets/models/block.glb", floor_wall_blocks[floor_wall_blocks_index].mesh_instance_array);
      ++floor_wall_blocks_index;
    }

    /* create floor */
    const size_t floor_row_count    = 11;
    const size_t floor_column_count = 13;
    first_floor_position = { first_block_position.x + block_offset_x, first_block_position.y, first_block_position.z + block_offset_z };
    for (size_t row_index=0; row_index < floor_row_count; ++row_index) {
      for (size_t column_index=0; column_index < floor_column_count; ++column_index) {
        floor_wall_blocks[floor_wall_blocks_index].orientation = identity_orientation;
        floor_wall_blocks[floor_wall_blocks_index].position    = { first_floor_position.x + ((float)column_index * block_offset_x), first_floor_position.y, first_floor_position.z + ((float)row_index * block_offset_z) };
        floor_wall_blocks[floor_wall_blocks_index].material_id = ( !(row_index % 2) == !(column_index % 2) ) ? floor_1_material_id : floor_2_material_id;
        create_graphics_mesh_instance_array_from_glb("assets/models/block.glb", floor_wall_blocks[floor_wall_blocks_index].mesh_instance_array);
        ++floor_wall_blocks_index;
      }
    }

    /* create stones */
    size_t stone_index = 0;
    for (size_t row_index=1; row_index < floor_row_count; row_index+=2) {
      for (size_t column_index=1; column_index < floor_column_count; column_index+=2) {
        all_stones[stone_index].orientation = identity_orientation;
        all_stones[stone_index].position    = { first_floor_position.x + ((float)column_index * block_offset_x), first_floor_position.y + block_offset_y, first_floor_position.z + ((float)row_index * block_offset_z) };
        all_stones[stone_index].material_id = stone_material_id;
        create_graphics_mesh_instance_array_from_glb("assets/models/block_rock.glb", all_stones[stone_index].mesh_instance_array);
        ++stone_index;
      }
    }

    /* create bricks and fire */
    size_t tile_index = 0;
    for (size_t row_index=0; row_index < floor_row_count; ++row_index) {
      for (size_t column_index=0; column_index < floor_column_count; ++column_index) {
        all_bricks[tile_index].orientation   = identity_orientation;
        all_bricks[tile_index].position      = { first_floor_position.x + ((float)column_index * block_offset_x), first_floor_position.y + block_offset_y, first_floor_position.z + ((float)row_index * block_offset_z) };
        all_bricks[tile_index].material_id_0 = brick_material_id_0;
        all_bricks[tile_index].material_id_1 = brick_material_id_1;
        create_graphics_mesh_instance_array_from_glb("assets/models/brick_block.glb", all_bricks[tile_index].mesh_instance_array);
        this->hide_brick(tile_index);

        all_bombs[tile_index].orientation   = identity_orientation;
        all_bombs[tile_index].position      = { first_floor_position.x + ((float)column_index * block_offset_x), first_floor_position.y + block_offset_y, first_floor_position.z + ((float)row_index * block_offset_z) };
        all_bombs[tile_index].material_id   = bomb_material_id;
        create_graphics_mesh_instance_array_from_glb("assets/models/bomb.glb", all_bombs[tile_index].mesh_instance_array);
        this->hide_bomb(tile_index);

        /*
        all_fire[tile_index].orientation = identity_orientation;
        all_fire[tile_index].position    = { first_floor_position.x + ((float)column_index * block_offset_x), first_floor_position.y + block_offset_y, first_floor_position.z + ((float)row_index * block_offset_z) };
        all_fire[tile_index].material_id = fire_material_id;
        create_graphics_skin_from_glb("assets/models/fire.glb", all_fire[tile_index].skin);
        all_fire[tile_index].skin.update_all(all_fire[tile_index].material_id);
        all_fire[tile_index].animations = load_animations_from_glb_file("assets/models/fire.glb");
        all_fire[tile_index].skin.play_animation(all_fire[tile_index].animations.first_animation, true);
        */

        ++tile_index;
      }
    }
  }

  void update_position(const Vector3f& position) {
    this->first_block_position = position;

    size_t floor_wall_blocks_index = 0;

    const Vector3f top_right_block_position = { first_block_position.x + (14.0f * block_offset_x), first_block_position.y, first_block_position.z };
    const Vector3f bottom_left_block_position = { first_block_position.x, first_block_position.y, first_block_position.z + (12.0f * block_offset_z) };
    for (size_t i=0; i < 15; ++i) {
      floor_wall_blocks[floor_wall_blocks_index].position = { first_block_position.x + ((float)i * block_offset_x), first_block_position.y, first_block_position.z };
      ++floor_wall_blocks_index;
      floor_wall_blocks[floor_wall_blocks_index].position = { first_block_position.x + ((float)i * block_offset_x), first_block_position.y + block_offset_y, first_block_position.z };
      ++floor_wall_blocks_index;
    }
    for (size_t i=1; i < 13; ++i) {
      floor_wall_blocks[floor_wall_blocks_index].position = { first_block_position.x, first_block_position.y, first_block_position.z + (i * block_offset_z) };
      ++floor_wall_blocks_index;

      floor_wall_blocks[floor_wall_blocks_index].position = { first_block_position.x, first_block_position.y + block_offset_y, first_block_position.z + ((float)i * block_offset_z) };
      ++floor_wall_blocks_index;
    }
    for (size_t i=1; i < 13; ++i) {
      floor_wall_blocks[floor_wall_blocks_index].orientation = identity_orientation;
      floor_wall_blocks[floor_wall_blocks_index].position = { top_right_block_position.x, top_right_block_position.y, top_right_block_position.z + ((float)i * block_offset_z) };
      ++floor_wall_blocks_index;

      floor_wall_blocks[floor_wall_blocks_index].position = { top_right_block_position.x, top_right_block_position.y + block_offset_y, top_right_block_position.z + ((float)i * block_offset_z) };
      ++floor_wall_blocks_index;
    }
    for (size_t i=1; i < 14; ++i) {
      floor_wall_blocks[floor_wall_blocks_index].position = { bottom_left_block_position.x + ((float)i * block_offset_x), bottom_left_block_position.y, bottom_left_block_position.z };
      ++floor_wall_blocks_index;

      floor_wall_blocks[floor_wall_blocks_index].position = { bottom_left_block_position.x + ((float)i * block_offset_x), bottom_left_block_position.y + block_offset_y, bottom_left_block_position.z };
      ++floor_wall_blocks_index;
    }

    const size_t floor_row_count    = 11;
    const size_t floor_column_count = 13;
    first_floor_position = { first_block_position.x + block_offset_x, first_block_position.y, first_block_position.z + block_offset_z };
    for (size_t row_index=0; row_index < floor_row_count; ++row_index) {
      for (size_t column_index=0; column_index < floor_column_count; ++column_index) {
        floor_wall_blocks[floor_wall_blocks_index].position    = { first_floor_position.x + ((float)column_index * block_offset_x), first_floor_position.y, first_floor_position.z + ((float)row_index * block_offset_z) };
        ++floor_wall_blocks_index;
      }
    }

    size_t stone_index = 0;
    for (size_t row_index=1; row_index < floor_row_count; row_index+=2) {
      for (size_t column_index=1; column_index < floor_column_count; column_index+=2) {
        all_stones[stone_index].position    = { first_floor_position.x + ((float)column_index * block_offset_x), first_floor_position.y + block_offset_y, first_floor_position.z + ((float)row_index * block_offset_z) };
        ++stone_index;
      }
    }

    size_t tile_index = 0;
    for (size_t row_index=0; row_index < floor_row_count; ++row_index) {
      for (size_t column_index=0; column_index < floor_column_count; ++column_index) {
        all_bricks[tile_index].position      = { first_floor_position.x + ((float)column_index * block_offset_x), first_floor_position.y + block_offset_y, first_floor_position.z + ((float)row_index * block_offset_z) };

        all_bombs[tile_index].position      = { first_floor_position.x + ((float)column_index * block_offset_x), first_floor_position.y + block_offset_y, first_floor_position.z + ((float)row_index * block_offset_z) };
        /*
        all_fire[tile_index].position    = { first_floor_position.x + ((float)column_index * block_offset_x), first_floor_position.y + block_offset_y, first_floor_position.z + ((float)row_index * block_offset_z) };
        */

        ++tile_index;
      }
    }
  }

  void update() {
    for (size_t i=0; i < std::size(floor_wall_blocks); ++i) {
      floor_wall_blocks[i].update();
    }
    for (size_t i=0; i < std::size(all_stones); ++i) {
      all_stones[i].update();
    }
    for (size_t i=0; i < std::size(all_bricks); ++i) {
      all_bricks[i].update();
    }

    for (size_t i=0; i < std::size(all_bombs); ++i) {
      all_bombs[i].update();
    }

    /*
    for (size_t i=0; i < std::size(all_fire); ++i) {
      all_fire[i].update();
    }
    */
  }
};


HandControllers* hands = new HandControllers();
BomberMan* test_man    = new BomberMan();
Board*      board      = new Board();

void head_pose_dependent_sim() {
  hands->update();
}

void SimulationState::init() {
  hands->init();

  directional_light.update_direction({0.0f, -1.0f, 0.0f});
  directional_light.update_color({1.0f, 1.0f, 1.0f});
  update_ambient_light_intensity(0.25f);

  test_man->init();
  test_man->skin.play_animation(test_man->animations.first_animation + 3, true);

  board->init();
}

void SimulationState::update() {
  PROFILE_FUNCTION();

  hands->update();
  test_man->update();
  board->update();

  if (input_state.left_hand_select) {
    DEBUG_LOG("left hand select\n");

    //play_audio_source(test_man->sound_id);
    //board->update_position(input_state.left_hand_transform.position);
    static size_t test_index = 0;
    //board->show_brick(test_index++);
    board->show_bomb(test_index++);
  }

  if (input_state.right_hand_select) {
    DEBUG_LOG("right hand select\n");
    platform_request_exit();
  }

  if (input_state.gamepad_select) {
    DEBUG_LOG("gamepad select\n");
  }

}

void SimulationState::exit() {
}
