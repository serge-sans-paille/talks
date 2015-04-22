TARGETS= print_vector named_params raii copy_and_swap if_init tuple_as_lambda print_unpack verbose_assert range_pair try_block_function curiously_recurring_template_pattern make_templated_class non_throwing_swap named_parameters multi_stmt_macro making_new_friends sfinae has_member has_member_function execute_around_pointer static_init_fiasco expression_templates most_vexing_parse enable_if initialization_list local_class_definition map_keys_to_vector file_to_string if_init_unique vertical_or_horizontal_storage
CLEANFILES=$(TARGETS)
CXXFLAGS=-std=c++14 -Wall
all:$(TARGETS)

clean:
	$(RM) $(CLEANFILES) *.o

