/**
 * Copyright (C) 2015 Laverna project Authors.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
/* global define */
define([
    'underscore',
    'q',
    'jquery',
    'backbone.radio',
    'marionette',
    'apps/notes/form/views/formView',
    'apps/notes/form/views/notebooks'
], function (_, Q, $, Radio, Marionette, View, NotebooksView) {
    'use strict';

    /**
     * Note form controller.
     *
     * Triggers the following events:
     * 1. channel: notesForm, event: stop
     *
     * Listens to the following events:
     * 1. channel: notes, event: save:after
     *
     * requests:
     * 1. channel: notes, request: save
     *    to save the changes.
     */
    var Controller = Marionette.Object.extend({

        initialize: function(options) {
            this.options = options;

            _.bindAll(this, 'show', 'redirect');

            // Fetch everything
            Q.all([
                Radio.request('notes', 'get:model:full', options),
                Radio.request('notebooks', 'get:all', _.pick(options, 'profile'))
            ])
            .spread(this.show)
            .catch(function() {
                console.error('Editor error', arguments);
            });

            // Events
            this.listenTo(Radio.channel('notes'), 'update:model', this.redirect);
            this.listenTo(Radio.channel('Confirm'), 'confirm', this.redirect);
            this.listenTo(Radio.channel('Confirm'), 'cancel', this.onConfirmCancel);
        },

        onDestroy: function() {
            this.stopListening();
            this.view.trigger('destroy');
        },

        show: function(note, notebooks) {
            var notebooksView;
            note = note[0];

            // Set document title
            Radio.request('global', 'set:title', note.get('title'));

            // Use behaviours that are appropriate for a device.
            if (Radio.request('global', 'is:mobile')) {
                delete View.prototype.behaviors.Desktop;
            }
            else {
                delete View.prototype.behaviors.Mobile;
            }

            this.view = new View({
                model     : note,
                profile   : note.profileId
            });

            // Show the view and trigger an event
            Radio.request('global', 'region:show', 'content', this.view);
            this.view.trigger('rendered');

            /*
             * Resolve the notebook ID.
             * If the current note doesn't have a notebook attached,
             * try to use one from the filter if it specifies a notebook.
             */
            var activeId = note.get('notebookId');
            if (activeId === '0' && this.options.filter === 'notebook') {
                activeId = this.options.query;
            }

            // Show notebooks selector
            notebooksView = new NotebooksView({
                collection : notebooks,
                activeId   : activeId
            });
            this.view.notebooks.show(notebooksView);

            // Listen to view events
            this.listenTo(this.view, 'save', this.save);
            this.listenTo(this.view, 'cancel', this.showConfirm);
        },

        save: function() {
            var self = this;

            return this.getContent()
            .then(function(data) {
                return Radio.request('notes', 'save', self.view.model, data);
            })
            .fail(function(e) {
                console.error('Error', e);
            });
        },

        getContent: function() {
            var self = this;

            return Radio.request('editor', 'get:data')
            .then(function(data) {
                return _.extend(data, {
                    title      : self.view.ui.title.val().trim(),
                    notebookId : self.view.notebooks.currentView.ui.notebookId.val().trim(),
                });
            });
        },

        /**
         * Warn a user that they have made some changes.
         */
        showConfirm: function() {
            var self = this;

            return this.getContent()
            .then(function(data) {
                var model = self.view.model.pick('title', 'content', 'notebookId');
                data  = _.pick(data, 'title', 'content', 'notebookId');

                if (_.isEqual(model, data)) {
                    return self.redirect();
                }

                Radio.request('Confirm', 'start', $.t('You have unsaved changes.'));
            })
            .fail(function(e) {
                console.error('form ShowConfirm', e);
            });
        },

        redirect: function() {
            if (!this.view.getOption('redirect')) {
                return;
            }

            // Stop the module and navigate back
            Radio.trigger('notesForm', 'stop');
            Radio.request('uri', 'back');
        },

        onConfirmCancel: function() {
            // Rebind keybindings again because TW bootstrap modal overrites ESC.
            this.view.trigger('bind:keys');
            this.view.options.isClosed = false;

            if (this.view.options.focus !== 'editor') {
                return this.view.ui[this.view.options.focus].focus();
            }
            Radio.trigger('editor', 'focus');
        }

    });

    return Controller;
});
